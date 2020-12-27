#include "chip8.hpp"
#include "display.hpp"
#include <iostream>

int main()
{
    chip8::display display {};
    chip8::vm chip8_vm {};
    chip8_vm.load_rom("ibmlogo.ch8");

    for (;;) {
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
        //        display.render(chip8_vm.screen);
        int count = 0;
        for (auto&& i : chip8_vm.screen) {
            std::cout << (int)i;
            if (++count % chip8::SCREEN_WIDTH == 0) std::cout << "\n";
        }
        std::cout << "OP:" << std::hex << (int)chip8_vm.current_op.type << std::endl;
    }
    return 0;
}
