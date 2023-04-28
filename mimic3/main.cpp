#include <assert.h>
#include <json/json.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "params.hpp"
#include "phonemize.hpp"
#include "synthesize.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // Log to stderr instead of stdout
    spdlog::set_default_logger(spdlog::stderr_color_st("mimic3"));
    spdlog::set_level(spdlog::level::debug);
    auto params = mimic3::params(argc, argv);
    if (!params.check())
    {
        spdlog::error("parameter check failed.");
        exit(EXIT_FAILURE);
    }

    std::filesystem::path modelPath = params.getModelPath();
    std::filesystem::path outputDirectory;

    spdlog::debug("Use modelPath: {}", modelPath.string());

    // Output directory for WAV files
    outputDirectory = std::filesystem::path(params.output_dir.c_str());
    spdlog::debug("Creating output directory: {}", outputDirectory.string());
    std::filesystem::create_directories(outputDirectory);

    // Verify model file exists
    std::ifstream modelFile(modelPath.c_str(), ios::binary);
    if (!modelFile.good())
    {
        spdlog::critical("Model not set correctly or doesn't exist: {}", modelPath.string());
        return EXIT_FAILURE;
    }

    // Load phoneme map
    std::optional<mimic3::PhonemeIdMap> idMap;
    /*std::filesystem::path idMapPath = modelPath.parent_path() / "phoneme_map.txt";
    std::ifstream idMapFile(idMapPath.c_str());
    if (idMapFile.good())
    {
        spdlog::debug("Loading phoneme id map: {}", idMapPath.string());
        idMap.emplace();
        mimic3::loadPhonemeIdMap(idMapFile, &idMap.value());
        spdlog::info("Loaded phoneme id map");
    }
    else
    {
        spdlog::warn("Missing phoneme id map: {}", idMapPath.string());
    }*/
    auto idPath = modelPath.parent_path() / "phonemes.txt";
    std::ifstream idFile(idPath.c_str());
    if (idFile.good())
    {
        spdlog::debug("Loading phoneme id: {}", idPath.string());
        idMap.emplace();
        mimic3::loadPhonemeIdMap(idFile, &idMap.value());
        spdlog::info("Loaded phoneme id");
    }
    else
    {
        spdlog::warn("Missing phoneme id: {}", idPath.string());
    }

    // Initialize
    mimic3::initializeEspeak();
    auto session = mimic3::createSession(modelPath);

    // -----------

    // Read lines from standard input.
    std::size_t lineIndex = 0;
    std::string line;
    while (getline(std::cin, line))
    {
        mimic3::mimic3_Request request;
        request.text = line;
        request.voice = params.language;
        request.noiseScale = params.noise_scale;
        request.lengthScale = params.length_scale;
        request.noiseW = params.noise_w;
        if (params.output_naming.empty())
        {
            request.outputPath = outputDirectory / params.output_naming;
        }
        else
        {
            request.outputPath = outputDirectory / (std::to_string(lineIndex) + ".wav");
        }
        mimic3::phonemize(request);
        mimic3::phonemes2ids(request, idMap);

        auto phonemeIds = &request.phonemeIds.value();
        if (phonemeIds->empty())
        {
            throw std::runtime_error("Empty phoneme ids");
        }

        spdlog::debug("Synthesizing audio with {} phoneme id(s)", phonemeIds->size());
        auto result = synthesize(session,
                                 request.outputPath,
                                 phonemeIds,
                                 request.noiseScale,
                                 request.lengthScale,
                                 request.noiseW,
                                 /*sampleRate*/ 22050);

        spdlog::info("Real-time factor: {} (infer={}, audio={})",
                     result.realTimeFactor,
                     result.inferSeconds,
                     result.audioSeconds);

        spdlog::info("Wrote {}", request.outputPath.string());
        lineIndex++;

        std::cout << line << std::endl;
    }

    mimic3::terminateEspeak();
    spdlog::info("Exiting");

    return EXIT_SUCCESS;
}
