#pragma once
#include <SDL2/SDL.h>
#include <array>
#include <cstdint>
#include <sstream>
#include <string>

#include "chip8.hpp"

namespace sdl8 {
using chip8::SCREEN_DIMS;
using chip8::SCREEN_HEIGHT;
using chip8::SCREEN_WIDTH;

class display {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

public:
    // scale factor
    static constexpr auto SCALE = 20;

    // draw function
    void render(std::array<uint8_t, SCREEN_DIMS>& data);

    display();
    ~display() noexcept;

    display(const display&) = delete;
    display& operator=(const display&) = delete;
    display(display&&) = delete;
    display& operator=(display&&) = delete;
};

} // namespace sdl8
