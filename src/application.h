#ifndef APPLICATION_H
#define APPLICATION_H 

#include "display.h"

class Application : public Display {
    public:
        Application(
            const std::string& windowTitle,
            unsigned int width, unsigned int height,
            uint32_t delay,
            unsigned int gapWidth,
            const std::string& fontPath,
            unsigned int fontSize,
            std::size_t size, 
            int maxValue);

        void renderInitialMessage(const std::string& message);
        void update(
            const std::vector<int>& numbers,
            std::size_t highlighted,
            std::size_t active,
            std::size_t numArrayAccesses , std::size_t numComparisons,
            bool isSorted = false);
};

#endif