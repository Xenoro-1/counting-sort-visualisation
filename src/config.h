#ifndef CONFIG_H
#define CONFIG_H 

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Config {
    public:
        std::string windowTitle;
        unsigned int width, height;
        uint32_t refreshRate;
        
        unsigned int gapWidth;

        int minValue, maxValue;
        std::size_t size;

        std::string fontPath;
        unsigned int fontSize;

        Config(const std::string &filePath) {
            loadConfig(filePath);
        }

    private:
        void loadConfig(const std::string &filePath) {
            std::ifstream file(filePath);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to load configuration: " + filePath);
            }

            json config;
            file >> config;

            /* Fetching the data */

            windowTitle = config["other"]["window_title"].get<std::string>();
            width = config["screen_resolution"]["width"].get<unsigned int>();
            height = config["screen_resolution"]["height"].get<unsigned int>();
            gapWidth = config["other"]["gap_width"].get<unsigned int>();
            size = config["sorting_variables"]["size"].get<std::size_t>();
            minValue = config["sorting_variables"]["min_value"].get<int>();
            maxValue = config["sorting_variables"]["max_value"].get<int>();
            refreshRate = config["timing_constants"]["refresh_rate"].get<uint32_t>();
            fontPath = config["other"]["font_path"].get<std::string>();
            fontSize = config["other"]["font_size"].get<unsigned int>();
        }
};

#endif