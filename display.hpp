#pragma once
#include <SDL2/SDL.h>
#include <cstdint>
#include <sstream>
#include <string>

namespace chip8 {


struct display {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;

    display(std::string name, const int screen_width, const int screen_height);
    ~display() noexcept;
    display(const display&) = delete;
    display& operator=(const display&) = delete;
    display(display&&) = delete;
    display& operator=(display&&) = delete;
};

void render_pixels_to_screen(int* pixels, display& screen);
} // namespace chip8
