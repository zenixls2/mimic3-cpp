#pragma once
#include <filesystem>
namespace mimic3
{
    struct params
    {
        int seed;
        int epochs;
        float noise_scale;
        float length_scale;
        float noise_w;
        // check available list from:
        // https://github.com/espeak-ng/espeak-ng/blob/master/docs/languages.md
        std::string language;
        std::string voice;
        std::string voices_dir;
        std::string output_dir;
        std::string output_naming;
        std::string id_delimiter;
        std::string auto_append_text;
        params();
        params(int argc, char **argv);
        params(params &t) = default;
        void print();
        void voices();
        bool check();
        std::filesystem::path getModelPath();

    private:
        void init();
    };
}

