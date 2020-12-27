#include "chip8.hpp"
#include <fstream>
#include <thread>
#include <chrono>

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

    case 0xA:
        index_reg = nn;
        break;

    // draw a sprite N rows talls at coords v[x] and v[y]
    // with the sprite data located with the I register
    // the sprite should modulo around the display but clip
    case 0xD:
        auto x_coord = v_registers[x] & SCREEN_WIDTH;
        auto y_coord = v_registers[y] & SCREEN_HEIGHT;
        v_registers[0xF] = 0;

        for (int i = 0; i < n; i++) {
            // stop drawing if y coord reaches the bottom
            if (y_coord >= SCREEN_HEIGHT) break;

            auto sprite = memory[index_reg];
            while(sprite) {
                // stop drawing row id reached edge of the screen
                if (x_coord >= SCREEN_WIDTH) break;
                // if the pixel is on in the sprite row xor it with the screen coords
                auto d_pix = screen[x_coord * SCREEN_WIDTH + y];

                // each bit in the sprite is a pixel
                auto s_pix = sprite & 1;
                if (s_pix == 1) {
                    screen[x_coord * SCREEN_WIDTH + y] ^= s_pix;
                    // if screen pixel was already on set v[F] reg to 1
                    if (d_pix == s_pix) v_registers[0xF] = 1;
                }
                sprite >>= 1;
                x_coord++;
            }
            y_coord++;
        }

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
