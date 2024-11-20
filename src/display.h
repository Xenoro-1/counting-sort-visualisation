#ifndef DISPLAY_H  
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

class Display {
    protected:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        TTF_Font* font = nullptr;

        std::string windowTitle;
        unsigned int width, height;
        uint32_t delay;

        unsigned int gapWidth;

        std::string fontPath;
        unsigned int fontSize;

        float barWidth;
        float yScale;

        /* Methods*/

        void init();

        void drawState(
            const std::vector<int>& numbers, 
            std::size_t highlighted,
            std::size_t active,
            bool isSorted = false);
        void renderText(
            const std::string& text, 
            int x, int y,
            int outlineWidth = 0);

    public:
        Display(
            const std::string& windowTitle,
            unsigned int width, unsigned int height,
            uint32_t delay,
            unsigned int gapWidth,
            const std::string& fontPath,
            unsigned int fontSize,
            std::size_t size, 
            int maxValue);
        ~Display();

        void waitForUserClick();
        void waitForWindowClose();
};

#endif