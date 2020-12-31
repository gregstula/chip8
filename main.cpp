#include "chip8.hpp"
#include "display.hpp"
#include <exception>
#include <iostream>

int main()
{
    try {
        chip8::display display {};
        chip8::vm vm {};

        vm.load_rom("ibmlogo.ch8");
        vm.start_timers();

        bool quit = false;
        while (!quit) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {

                if (event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }
                else if (event.type == SDL_KEYDOWN) {
                }
                else if (event.type == SDL_KEYUP) {
                }
            }
            vm.tick();
            display.render(vm.screen);
            /*            int count = 0;
            for (auto&& i : vm.screen) {
                std::cout << (int)i;
                if (++count % chip8::SCREEN_WIDTH == 0) std::cout << "\n";
            }
            std::cout << "OP:" << std::hex << (int)vm.current_op.type << std::endl;
*/
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
