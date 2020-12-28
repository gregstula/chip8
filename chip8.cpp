#include "chip8.hpp"
#include <chrono>
#include <fstream>
#include <thread>

namespace chip8 {
using namespace std::chrono_literals;
void vm::load_rom(std::string path)
{
    std::ifstream rom(path, std::ios::binary | std::ios::ate);
    auto size = rom.tellg();
    rom.seekg(0, std::ios::beg);
    rom.read(reinterpret_cast<char*>(memory.data() + 512), size);
    program_counter = 512;
}

void vm::fetch()
{
    current_op = instruction(memory[program_counter], memory[program_counter + 1]);
    program_counter += 2;
}

void vm::execute()
{
    auto&& [type, x, y, n, nn, nnn] = current_op;

    switch (type) {
    case 0x0:
        // clear screen
        if (nn == 0xE0) {

            screen.fill(0);
        }

        // return from subroutine
        else if (nn == 0xEE) {
            program_counter = stack.top();
            stack.pop();
        }
        break;

    // jump to nnn
    case 0x1:
        program_counter = nnn;
        break;

    // call subroutine at nnn
    case 0x2:
        stack.push(program_counter);
        program_counter = nnn;
        break;

    // set the register vX to nn
    case 0x6:
        v_registers[x] = nn;
        break;

    // add nn to register v[x]
    case 0x7:
        v_registers[x] += nn;
        break;

    // set i register;
    case 0xA:
        index_reg = nnn;
        break;

    // draw a sprite N rows talls at coords v[x] and v[y]
    // with the sprite data located with the I register
    // the sprite should modulo around the display but clip
    case 0xD: {
        auto x_coord = v_registers[x];
        auto y_coord = v_registers[y];
        v_registers[0xF] = 0;

        for (int i = 0; i < n; i++) {
            // stop drawing if y coord reaches the bottom
            if (y_coord >= SCREEN_HEIGHT) break;

            auto sprite = memory[index_reg + i];
            for (int j = 0; j < 8; j++) {
                // if the pixel is on in the sprite row xor it with the screen coords
                auto coords = ((x_coord + j) + SCREEN_WIDTH * (y_coord + i)) % 2048;
                auto d_pix = screen[coords];
                // each bit in the sprite is a pixel
                auto s_pix = sprite & (0b10000000 >> j);
                if(s_pix) {
                    screen[coords] ^= 1;
                    // if was originally 1 set to 1
                    v_registers[0xF] = d_pix;
                }
            }
        }
        break;
    }
    default:
        break;
    }
}

void vm::tick()
{
    fetch();
    execute();
    std::this_thread::sleep_for(60ms);
}

} // namespace chip8
