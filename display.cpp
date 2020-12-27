#include "display.hpp"
namespace chip8 {

display::display(std::string name, const int screen_width, const int screen_height) : SCREEN_WIDTH(screen_width), SCREEN_HEIGHT(screen_height)
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::stringstream ss;
        ss << "Could not initialize SDL2: " << SDL_GetError();
        throw std::runtime_error(ss.str());
    }

    // Try to create window
    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_Quit();
        std::stringstream ss;
        ss << "Could not create window with SDL2: " << SDL_GetError();
        throw std::runtime_error(ss.str());
    }

    // Try to create renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        SDL_Quit();

        std::stringstream ss;
        ss << "Could not create renderer with SDL2: " << SDL_GetError();
        throw std::runtime_error(ss.str());
    }

    // create texture
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_HEIGHT, SCREEN_WIDTH);
}

display::~display() noexcept
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

void render_pixels_to_screen(int* pixel_data, display& screen)
{
}

}
