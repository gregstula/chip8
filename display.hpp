#pragma once
#include <SDL2/SDL.h>
#include <array>
#include <cstdint>
#include <sstream>
#include <string>

#include "chip8.hpp"

namespace chip8 {
class display {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

public:
    // scale factor
    static constexpr auto SCALE = 10;

    // draw function
    void render(std::array<uint8_t, SCREEN_DIMS>& data);

    display();
    ~display() noexcept;

    display(const display&) = delete;
    display& operator=(const display&) = delete;
    display(display&&) = delete;
    display& operator=(display&&) = delete;
};

} // namespace chip8
