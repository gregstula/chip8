#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
// clang-format off

namespace sdl8 {
// scancodes to keypad
// chip-8 devices had keypads  like this
// 1 2 3 C
// 4 5 6 D
// 7 8 9 E
// A 0 B F
const std::unordered_map<SDL_Scancode, uint8_t> scancode_to_keypad = {
    { SDL_SCANCODE_1, 0x1 }, { SDL_SCANCODE_2, 0x2 }, { SDL_SCANCODE_3, 0x3 }, { SDL_SCANCODE_4, 0xC },
    { SDL_SCANCODE_Q, 0x4 }, { SDL_SCANCODE_W, 0x5 }, { SDL_SCANCODE_E, 0x6 }, { SDL_SCANCODE_R, 0xD },
    { SDL_SCANCODE_A, 0x7 }, { SDL_SCANCODE_S, 0x8 }, { SDL_SCANCODE_D, 0x9 }, { SDL_SCANCODE_F, 0xE },
    { SDL_SCANCODE_Z, 0xA }, { SDL_SCANCODE_X, 0x0 }, { SDL_SCANCODE_C, 0xB }, { SDL_SCANCODE_V, 0xF }
};

} // namespace sdl8
