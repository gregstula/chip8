#include "display.hpp"
#include <stdexcept>
#include <cstring>

namespace chip8 {

display::display() {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::stringstream ss;
        ss << "Could not initialize SDL2! " << SDL_GetError();
        throw std::runtime_error(ss.str());
    }

    // Try to create window
    window = SDL_CreateWindow("chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_Quit();
        std::stringstream ss;
        ss << "Could not create window! SDL2 ERROR: " << SDL_GetError();
        throw std::runtime_error(ss.str());
    }

    // Try to create renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        SDL_Quit();

        std::stringstream ss;
        ss << "Could not create renderer! SDL2 ERROR: " << SDL_GetError();
        throw std::runtime_error(ss.str());
    }

    // create texture
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_HEIGHT, SCREEN_WIDTH);
}

display::~display() noexcept
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void display::render(std::array<int,SCREEN_DIMS>& pixels)
{
    // Copy emulator pixels to texture pixels on GPU
    void* texture_pixels; // used by lock function
    int pitch;
    // Lock texture pixels to be write only
    if (SDL_LockTexture(texture, nullptr, &texture_pixels, &pitch) != 0) {
         std::stringstream ss;
        ss << "Could not lock texture! SDL2 ERROR: " << SDL_GetError();
        throw std::runtime_error(ss.str());
    }
    // copy the pixels
    std::memcpy(texture_pixels, pixels.data(), pixels.size());
    // unlock the texture applying changes.
    SDL_UnlockTexture(texture);

   SDL_RenderClear(renderer);
   SDL_RenderCopy(renderer, texture, nullptr, nullptr);
   SDL_RenderPresent(renderer);
}

