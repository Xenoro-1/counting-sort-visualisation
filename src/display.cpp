#include "display.h"

#include <stdexcept>
#include <algorithm>

Display::Display(
    const std::string& windowTitle,
    unsigned int width, unsigned int height,
    uint32_t delay,
    unsigned int gapWidth,
    const std::string& fontPath,
    unsigned int fontSize,
    std::size_t size, 
    int maxValue) : 
        windowTitle (windowTitle),
        width(width), height(height), 
        delay(delay),
        gapWidth(gapWidth),
        fontPath(fontPath),
        fontSize(fontSize),
        barWidth(0.0f), 
        yScale(0.0f){

            init();

            barWidth = (static_cast<float>(width) - static_cast<float>(size) * this->gapWidth) / static_cast<float>(size);
            yScale = static_cast<float>(height) / static_cast<float>(maxValue);
}

Display::~Display() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    TTF_Quit();
    SDL_Quit();
}

void Display::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw std::runtime_error("SDL initialization failed: \n" + std::string(SDL_GetError()));
    }

    if (TTF_Init() != 0) {
        throw std::runtime_error("TTF initialization failed: \n" + std::string(TTF_GetError()));
    }

    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        throw std::runtime_error("Error loading font: \n" + std::string(TTF_GetError()));
    }

    if(SDL_CreateWindowAndRenderer(
        width, height, 
        0, 
        &window, &renderer) != 0) {
            throw std::runtime_error("Error creating window and renderer: " + std::string(SDL_GetError()));
        }       
    
    SDL_SetWindowTitle(window, windowTitle.c_str());
    SDL_RenderSetScale(renderer, 1.0f, 1.0f);
}

void Display::waitForUserClick() {
    SDL_Event e;
    bool hasClicked = false;

    while (!hasClicked) {
        SDL_WaitEvent(&e);

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            hasClicked = true;
        }
    }
}

void Display::waitForWindowClose() {
    SDL_Event e;
    bool quit = false;

    while (!quit) {
        SDL_WaitEvent(&e);

        if (e.type == SDL_QUIT) {
            quit = true;
        }
    }
}

void Display::drawState(
    const std::vector<int>& numbers, 
    std::size_t highlighted,
    std::size_t active,
    bool isSorted) {

    std::size_t index = 0;
    for (const auto& n : numbers) {
        float xPos = index * (barWidth + gapWidth);
        float barHeight = n * yScale;

        if (isSorted) {
            if (highlighted >= index) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  
            }
        } else {
            if (highlighted == index) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  
            } else if (active == index) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); 
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
            }
        }

        SDL_FRect barRect = {xPos, static_cast<float>(height) - barHeight, barWidth, barHeight};
        SDL_RenderFillRectF(renderer, &barRect);

        ++index; 
    }
}

void Display::renderText(
    const std::string& text, 
    int x, int y,
    int outlineWidth) {
        
        if (outlineWidth > 0) {
            for (int dx = -outlineWidth; dx <= outlineWidth; dx++) {
                for (int dy = -outlineWidth; dy <= outlineWidth; dy++) {
                    if (dx != 0 || dy != 0) {
                        SDL_Surface* outlineSurface = TTF_RenderText_Solid(font, text.c_str(), SDL_Color({0, 0, 0, 255}));
                        SDL_Texture* outlineTexture = SDL_CreateTextureFromSurface(renderer, outlineSurface);

                        SDL_Rect outlineRect = {x + dx, y + dy, outlineSurface->w, outlineSurface->h};
                        SDL_RenderCopy(renderer, outlineTexture, nullptr, &outlineRect);

                        SDL_FreeSurface(outlineSurface);
                        SDL_DestroyTexture(outlineTexture);
                    }
                }
            }
        }
        
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), SDL_Color({255, 255, 255, 255}));
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
}