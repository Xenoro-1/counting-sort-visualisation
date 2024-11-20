#include "application.h"
#include "config.h"
#include "filler.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <vector>


/* Predefined variables */

#define CONFIG_PATH "../assets/config.json"
#define LOG_PATH "./log.txt"

void sortAndUpdateUI(
    Application& application, 
    Config& config,
    std::vector<int>& numbers, 
    std::size_t& numArrayAccesses,
    std::size_t& numComparisons) {

        /* Sorting algorithm */

        std::vector<int> n(config.maxValue, 0);
        for (int i = 0; i < config.size; i++) {
            numArrayAccesses++; 
            n[numbers[i] - 1]++; 
        }

        for (int i = config.minValue; i < config.maxValue; i++) {
            numArrayAccesses++;  
            n[i] += n[i - 1];  
        }

        std::vector<int> k(config.size);
        k = numbers;

        for (int i = config.size - 1; i >= 0; i--) {
            numArrayAccesses += 2;  

            k[n[numbers[i] - 1] - 1] = numbers[i]; 
            n[numbers[i] - 1]--;

            application.update(
                k, 
                i, -1,  
                numArrayAccesses, 
                numComparisons);
        }

        numbers = k;
}

int main(int argc, char *argv[]) {
    try {
        Config config(CONFIG_PATH);

        std::vector<int> numbers;
        numbers.reserve(config.size);

        Filler random(
            config.minValue, 
            config.maxValue);
        random.fill(
            numbers, 
            config.size);

        Application application(
            config.windowTitle, 
            config.width, config.height, 
            config.refreshRate, 
            config.gapWidth,
            config.fontPath,
            config.fontSize, 
            config.size, 
            config.maxValue);

        application.renderInitialMessage("Press to begin");
        application.waitForUserClick();

        std::size_t numArrayAccesses  = 0, numComparisons = 0;
        sortAndUpdateUI(
            application,
            config, 
            numbers, 
            numArrayAccesses, 
            numComparisons);

        /* Update the color after the sorting process finishes */

        for (std::size_t i = 0; i < config.size; i++) {
            application.update(
                numbers, 
                i, 0, 
                numArrayAccesses, 
                numComparisons, 
                true);
        }

        application.waitForWindowClose();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl; 

        try {
            std::ofstream logFile(LOG_PATH, std::ios::app);

            if (!logFile.is_open()) {
                std::cerr << "Error: Could not open the file: " << LOG_PATH << std::endl;
                return EXIT_FAILURE;
            }

            auto currentTime = std::chrono::system_clock::now();
            auto currentTimeInSeconds = std::chrono::system_clock::to_time_t(currentTime);

            std::ostringstream formattedTimestamp;
            formattedTimestamp << std::put_time(std::localtime(&currentTimeInSeconds), "%Y-%m-%d %H:%M:%S");

            logFile << "[" << formattedTimestamp.str() << "] " << " " << e.what() << std::endl;

        } catch (const std::exception& logEx) {
            std::cerr << "Logging failed: " << logEx.what() << std::endl;
        }

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}