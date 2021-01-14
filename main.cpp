#include "chip8.hpp"
#include "display.hpp"
#include "keypad.hpp"
#include <exception>
#include <iostream>
#include <string>
#include <unordered_map>

int main(int argc, char* argv[])
{
    try {
        if (argc <= 1) {
            std::cout << "Use: ./chip8 [rom]" << std::endl;
            return 0;
        }

        sdl8::display display {};
        chip8::vm vm {};
        vm.load_rom(argv[1]);
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
                    // convert scancode to emulator keymap string;
                    auto lookup = sdl8::scancode_to_keypad.find(event.key.keysym.scancode);
                    if (lookup != sdl8::scancode_to_keypad.end()) {
                        auto key = lookup->second;
                        // key was pressed
                        vm.keypad[key] = true;
                    }
                }
                else if (event.type == SDL_KEYUP) {
                    // convert scancode to emulator keymap string;
                    auto lookup = sdl8::scancode_to_keypad.find(event.key.keysym.scancode);
                    if (lookup != sdl8::scancode_to_keypad.end()) {
                        auto key = lookup->second;
                        // key was pressed
                        vm.keypad[key] = false;
                    }
                }
            }
            vm.tick();
            display.render(vm.screen);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
