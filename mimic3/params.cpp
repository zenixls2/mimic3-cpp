#include "params.hpp"

#include <cstdlib>
#include <iostream>
namespace mimic3
{
    void params::init()
    {
        language = "en";
        voice = "en_UK/apope_low";
        std::string home = getenv("HOME");
        voices_dir = home + "/.local/share/mycroft/mimic3/voices";
        output_dir = ".";
        output_naming = "result.wav";
        id_delimiter = "|";
        noise_scale = 0.667f;
        length_scale = 1.0f;
        noise_w = 0.8f;
        auto_append_text = ".";
    }

    params::params()
    {
        init();
    }
    params::params(int argc, char **argv)
    {
        init();
        for (int i = 0; i < argc; i++)
        {
            std::string arg = argv[i];
            std::cout << arg << std::endl;
            if (arg == "-h" || arg == "--help")
            {
                print();
                exit(EXIT_SUCCESS);
            }
            else if (arg == "-v" || arg == "--voice")
            {
                voice = argv[++i];
            }
            else if (arg == "-s" || arg == "--speaker")
            {
                // pass
                ++i;
            }
            else if (arg == "--voices-dir")
            {
                voices_dir = argv[++i];
            }
            else if (arg == "--output-dir")
            {
                output_dir = argv[++i];
            }
            else if (arg == "--voices")
            {
                voices();
            }
            else if (arg == "--output-naming")
            {
                output_naming = argv[++i];
            }
            else if (arg == "--id-delimiter")
            {
                id_delimiter = argv[++i];
            }
            else if (arg == "--noise-w")
            {
                noise_w = std::stof(argv[++i]);
            }
            else if (arg == "--noise-scale")
            {
                noise_scale = std::stof(argv[++i]);
            }
            else if (arg == "--length-scale")
            {
                length_scale = std::stof(argv[++i]);
            }
        }
    }

    void params::print()
    {
        std::cout
            << "mimic3 [-h]"
            << " [--language LANG]"
            << " [--noise-scale NOISE_SCALE]"
            << " [--length-scale LENGTH_SCALE]"
            << " [--noise-w NOISE_W]"
            << " [--vocie VOICE]"
            << " [--voices-dir VOICES_DIR]"
            << " [--voices]"
            << " [--output-dir OUTPUT_DIR]"
            << " [--output-naming {text,time,id}]\n\n"
            << "Mimic 3 commad-line interface\n"
            << "positional arguments:\n\n"
            << "optional arguments:\n"
            << "  -h, --help: show this help message and exit\n"
            << "  --language LANG: language abbreviation pass to espeak-ng, default is en\n"
            << "      Check: https://github.com/espeak-ng/espeak-ng/blob/master/docs/languages.md\n"
            << "  --voice VOICE, -v VOICE: Name of voice (expected in <voices-dir>/<language>)\n"
            << "  --voice-dir VOICE: Directory with voices (format is <language>/<voice_name>)\n"
            << "  --vocies: List available voices\n"
            << "  --output-dir OUTPUT_DIR: Directory to write WAV file(s)\n"
            << "  --output-naming {text,time,id}: Naming scheme for output WAV files\n"
            << "  --id-delimiter ID_DELIMITER: Delimiter between id and text in lines (default: "
               "|)\n"
            << "  --length-scale LENGTH_SCALE: Length scale (1.0 is default speed, 0.5 is 2x "
               "faster)\n"
            << "  --noise-scale NOISE_SCALE: Noise scale [0-1], default is 0.667\n"
            << "  --noise-w NOISE_W: Variation in cadence [0-1], default is 0.8\n";
    }

    void params::voices()
    {
        std::cout << "available voices: " << std::endl;
    }

    bool params::check()
    {
        return !(output_dir.empty() || output_naming.empty() || voice.empty() ||
                 voices_dir.empty() || id_delimiter.empty());
    }

    std::filesystem::path params::getModelPath()
    {
        std::filesystem::path modelPath(voices_dir.c_str());
        return modelPath / voice / "generator.onnx";
    }
}
