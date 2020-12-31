#pragma once
#include <array>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <stack>
#include <string>
#include <thread>

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
    void start_timers();
    void load_rom(std::string path);
    void tick();

    std::array<std::uint8_t, 4096> memory;
    std::array<std::uint8_t, SCREEN_DIMS> screen;
    std::uint16_t program_counter;
    std::uint16_t index_reg;
    std::stack<std::uint16_t> stack;
    std::array<std::uint8_t, 15> V;

    // timers
    std::atomic<std::uint8_t> delay_timer { 0 };
    std::atomic<std::uint8_t> sound_timer { 0 };

    // current op
    instruction current_op { 0, 0 };

    void launch_delay_timer();
    void launch_timer(std::atomic<std::uint8_t>& timer);
    void fetch();
    void execute();
};

} // namespace chip8
