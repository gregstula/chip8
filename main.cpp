#include "display.hpp"
#include "chip8.hpp"
#include <iostream>

int main()
{
    chip8::display display{};
    chip8::vm chip8_vm{};
    chip8_vm.load_rom("ibmlogo.ch8");

    for(;;) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {

                break;
            }
            else if (event.type == SDL_KEYDOWN) {

            }
            else if (event.type == SDL_KEYUP) {

            }
        }
        chip8_vm.tick();
        //display.render(vm.screen);
        for (auto&& i : chip8_vm.screen) {
            std::cout << i;
        }
        std::cout << std::endl;
    }
    return 0;
}
