#pragma once
#include <array>
#include <stack>
#include <string>

namespace chip8 {

constexpr int SCREEN_WIDTH = 64;
constexpr int SCREEN_HEIGHT = 32;
constexpr int SCREEN_DIMS = SCREEN_WIDTH * SCREEN_HEIGHT;

struct instruction {
    uint8_t type;
    uint8_t x;
    uint8_t y;
    uint8_t n;
    uint8_t nn;
    uint16_t nnn;

    instruction(uint8_t byte1, uint8_t byte2)
    {
        // first nibble
        type = (byte1 >> 4) & 0xF;
        // second nibble
        x = byte1 & 0xF;
        // third nibble
        y = (byte2 >> 4) & 0xF;
        // fourth nibble
        n = byte2 & 0xF;
        // second byte
        nn = byte2;
        // 12 bits, nibbles 2-4
        nnn = (x << 8) | byte2;
    }
};

struct vm {
    std::array<uint8_t, 4096> memory;
    std::array<uint8_t, SCREEN_DIMS> screen;
    uint16_t program_counter;
    uint16_t index_reg;
    std::stack<uint16_t> stack;
    std::array<uint8_t, 15> v_registers;
    instruction current_op { 0, 0 };

    void load_rom(std::string path);
    void fetch();
    void execute();
    void tick();
};

} // namespace chip8
