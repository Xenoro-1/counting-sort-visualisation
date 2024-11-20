#include "application.h"

Application::Application(
    const std::string& windowTitle,
    unsigned int width, unsigned int height,
    uint32_t delay,
    unsigned int gapWidth,
    const std::string& fontPath,
    unsigned int fontSize,
    std::size_t size, 
    int maxValue) : Display(
        windowTitle, 
        width, height, 
        delay,
        gapWidth,
        fontPath,
        fontSize, 
        size, 
        maxValue){
}

void Application::renderInitialMessage(const std::string& message) {
    renderText(message, 0, 0);
    SDL_RenderPresent(renderer);
}

void Application::update(
    const std::vector<int>& numbers,
    std::size_t highlighted,
    std::size_t active,
    std::size_t numArrayAccesses, 
    std::size_t numComparisons,
    bool isSorted) {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawState(numbers, highlighted, active, isSorted);

        std::string accessText = "Array Accesses: " + std::to_string(numArrayAccesses );
        renderText(accessText, 0, 0, 1); 

        std::string comparisonsText  = "Comparisons: " + std::to_string(numComparisons);
        renderText(comparisonsText, 0, 15, 1);

        SDL_RenderPresent(renderer);
        SDL_Delay(isSorted ? 1 : delay);
}