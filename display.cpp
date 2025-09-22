#include "display.hpp"
#include <cstring>
#include <stdexcept>

namespace sdl8 {

display::display()
{
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
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
}

display::~display() noexcept
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void display::render(std::array<uint8_t, SCREEN_DIMS>& pixels)
{
    // Copy emulator pixels to texture pixels on GPU
    uint32_t* texture_pixels = nullptr; // used by lock function
    int pitch;
    // Lock texture pixels to be write only
    if (SDL_LockTexture(texture, nullptr, reinterpret_cast<void**>(&texture_pixels), &pitch) != 0) {
        std::stringstream ss;
        ss << "Could not lock texture! SDL2 ERROR: " << SDL_GetError();
        throw std::runtime_error(ss.str());
    }
    // copy the pixels
    for (size_t i = 0; i < pixels.size(); i++) {
        // white for on, black for off
        texture_pixels[i] = pixels[i] != 0 ? 0xFFFFFFFF : 0xFF000000;
    }

    // unlock the texture applying changes.
    SDL_UnlockTexture(texture);

    // SDL_Rect dest_rect = {0,0,SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE};
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

} // namespace sdl8
