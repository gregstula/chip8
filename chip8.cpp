#include "chip8.hpp"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <random>
#include <unordered_map>

namespace chip8 {
using namespace std::chrono_literals;

// hellish C++ rand() replacement
std::mt19937 make_seeded_rng()
{
    std::random_device dev_rand; // reads from /dev/random and returns an int
    std::array<std::uint32_t, std::mt19937::state_size> a; // array to hold all 600+ states needed to properly seed "mersenne twister" rng
    std::generate(a.begin(), a.end(), std::ref(dev_rand)); // generate the numbers for the array using the random device
    std::seed_seq seed(a.begin(), a.end()); // create the seed sequence from the array of random ints
    // return the actual mersenne twister and hope RVO moves it
    return { std::mt19937(seed) };
}

// over-engineered rng function for the 0xC instruction to get a random int 0-255
// mt19937 aka mersenne twister is expensive to make so we only make it once
// that's why we use a seperate function to make it and save it statically
uint8_t rng()
{

    thread_local static auto rng = make_seeded_rng();
    std::uniform_int_distribution<int> dist(0, 255);
    return dist(rng);
}

// constructor
vm::vm()
{
    // fonts
    const std::array<uint8_t, 5 * 16> fonts = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20,
        0x60,
        0x20,
        0x20,
        0x70, // 1
        0xF0,
        0x10,
        0xF0,
        0x80,
        0xF0, // 2
        0xF0,
        0x10,
        0xF0,
        0x10,
        0xF0, // 3
        0x90,
        0x90,
        0xF0,
        0x10,
        0x10, // 4
        0xF0,
        0x80,
        0xF0,
        0x10,
        0xF0, // 5
        0xF0,
        0x80,
        0xF0,
        0x90,
        0xF0, // 6
        0xF0,
        0x10,
        0x20,
        0x40,
        0x40, // 7
        0xF0,
        0x90,
        0xF0,
        0x90,
        0xF0, // 8
        0xF0,
        0x90,
        0xF0,
        0x10,
        0xF0, // 9
        0xF0,
        0x90,
        0xF0,
        0x90,
        0x90, // A
        0xE0,
        0x90,
        0xE0,
        0x90,
        0xE0, // B
        0xF0,
        0x80,
        0x80,
        0x80,
        0xF0, // C
        0xE0,
        0x90,
        0x90,
        0x90,
        0xE0, // D
        0xF0,
        0x80,
        0xF0,
        0x80,
        0xF0, // E
        0xF0,
        0x80,
        0xF0,
        0x80,
        0x80 // F
    };

    // Copy font data to memory
    std::copy(fonts.begin(), fonts.end(), memory.begin());

    keypad = {
        { "1", false }, { "2", false }, { "3", false }, { "C", false }, { "4", false }, { "5", false }, { "6", false }, { "D", false }, { "7", false }, { "8", false }, { "9", false }, { "E", false }, { "A", false }, { "0", false }, { "B", false }, { "F", false }
    };
}

// start delay timer on a seperate thread
void vm::launch_timer(std::atomic<std::uint8_t>& timer)
{
    std::thread([&]() {
        for (;;) {
            auto target_time = std::chrono::high_resolution_clock::now();
            while (timer) {
                target_time += 16ms;
                std::this_thread::sleep_until(target_time);
                --delay_timer;
            }
            std::this_thread::yield();
        }
    }).detach();
}

void vm::start_timers()
{
    launch_timer(delay_timer);
    launch_timer(sound_timer);
}

// Slurps the rom into the vm memory
void vm::load_rom(std::string path)
{
    std::ifstream rom(path, std::ios::binary | std::ios::ate);
    auto size = rom.tellg();
    rom.seekg(0, std::ios::beg);
    rom.read(reinterpret_cast<char*>(memory.data() + 512), size);
    program_counter = 512;
}

// Fetches the next instruction and increments the program counter
void vm::fetch()
{
    current_op = instruction(memory[program_counter], memory[program_counter + 1]);
    program_counter += 2;
}

// Execute opcode switch
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

    // skip instruction if v[x] is nn
    case 0x3:
        if (V[x] == nn) program_counter += 2;
        break;
    // skip instruction if v[x] is not nn
    case 0x4:
        if (V[x] != nn) program_counter += 2;
        break;
    // skip instruction if v[x] == v[y]
    case 0x5:
        if (V[x] == V[y]) program_counter += 2;
        break;

    // set the register vX to nn
    case 0x6:
        V[x] = nn;
        break;

    // add nn to register v[x]
    case 0x7:
        V[x] += nn;
        break;

    // logical operations
    case 0x8:
        switch (n) {
        // set v[x] to v[y]
        case 0x0:
            V[x] = V[y];
            break;
        // bitwise OR v[x] v[y]
        case 0x1:
            V[x] |= V[y];
            break;
        // bitwise AND v[x] v[y]
        case 0x2:
            V[x] &= V[y];
            break;
        // bitwise XOR v[x] v[y]
        case 0x3:
            V[x] ^= V[y];
            break;
        // add v[x] v[y] set v[F] to 1 if carry
        // and preserve the lower 8 bits of addition only
        case 0x4: {
            int result = V[x] + V[y];
            if (result > 255) {
                V[0xF] = 1;
                result = result & 0xFF;
            }
            V[x] = result;
            break;
        }
        // subtraction v[x] - v[y]. if v[x] > v[y] then v[f] is set to 1
        case 0x5:
            V[0xF] = V[x] > V[y];
            V[x] -= V[y];
            break;

        // shift right by 1
        case 0x6:
            V[0xF] = V[x] & 1;
            V[x] /= 2;
            break;

            // subtraction v[y] - v[x]. if v[y] > v[x] then v[f] is set to 1
        case 0x7:
            V[0xF] = V[x] < V[y];
            V[x] = V[y] - V[x];
            break;

        // shift left
        case 0xE:
            V[0xF] = V[x] & 0b10000000;
            V[x] *= 2;
            break;

        } // switch
        break;

    // skip next intruction if Vx != Vy
    case 0x9:
        if (V[x] != V[y]) program_counter += 2;
        break;

    // set i register;
    case 0xA:
        index_reg = nnn;
        break;

    // jump to nnn + v[0]
    case 0xB:
        program_counter = nnn + V[0];
        break;
    // set V[x] to a random byte which is AND of nn
    case 0xC:
        V[x] = rng() & nn;
        break;

    // draw a sprite N rows talls at coords v[x] and v[y]
    // with the sprite data located with the I register
    // the sprite should modulo around the display but clip
    case 0xD: { // variable scope
        auto x_coord = V[x];
        auto y_coord = V[y];
        V[0xF] = 0;

        /* Example Sprite letter E of height n = 7
                 bit 7 6 5 4 3 2 1 0
        -------+--------------------
        byte 1 |     0 1 1 1 1 1 0 0
        byte 2 |     0 1 0 0 0 0 0 0
        byte 3 |     0 1 0 0 0 0 0 0
        byte 4 |     0 1 1 1 1 1 0 0
        byte 5 |     0 1 0 0 0 0 0 0
        byte 6 |     0 1 0 0 0 0 0 0
        byte 7 |     0 1 1 1 1 1 0 0
        */
        for (int byte = 0; byte < n; byte++) {
            // stop drawing if y coord reaches the bottom
            if (y_coord >= SCREEN_HEIGHT) break;

            auto sprite = memory[index_reg + byte];
            // byte length
            for (int bit = 0; bit < 8; bit++) {
                // if the pixel is on in the sprite row xor it with the screen coords
                auto coords = ((x_coord + bit) + SCREEN_WIDTH * (y_coord + byte)) % 2048;
                auto d_pix = screen[coords];
                // each bit in the sprite is a pixel
                auto s_pix = sprite & (0b10000000 >> bit);
                if (s_pix) {
                    screen[coords] ^= 1;
                    // if was originally 1 set to 1
                    V[0xF] = d_pix;
                }
            }
        }
        break;
    } // variable scope

    // two instructions here
    case 0xE:
        switch (nn) {

        // Skip next instruction if the key with the balue of V[x] is pressed
        case 0x9E:
            // TODO
            break;
        // Skip next instruction if the key with the balue of V[x] is NOT pressed
        // TODO
        case 0xA1:
            break;
        }
        break;

    case 0xF:
        switch (nn) {
        // set Vx to delay timer
        case 0x07:
            V[x] = delay_timer;
            break;
            // Block execution until keypress. Store keypress in V[x]
        case 0x0A:
            // TODO
            break;
        // set delay timer to V[x]
        case 0x15:
            delay_timer = V[x];
            break;
        // Set sound timer to V[x]
        case 0x18:
            sound_timer = V[x];
            break;
        // increment index register by V[x]
        case 0x1E:
            index_reg += V[x];
            break;
        // set I to the Font that is the hex value of V[x]
        case 0x29:
            // TODO
            break;
        //  "Binary-coded decimal conversion"
        // Splits the number at V[x] up by the hundreths place, tens place
        // and once place and stores them at an offset from the Index register
        // hundreths place at I, tens at I+1, and ones at I+2
        // Example with 155: 100 at address I, 50 at address I + 1, and 5 at I + 2
        case 0x33: {
            auto num = V[x];
            int i = 2;
            while (num) {
                memory[index_reg + i--] = num % 10;
                num /= 10;
            }
        } break;
        // Store values froms registers V0 through Vx in memory starting at I
        case 0x55:
            for (int i = 0; i < x; i++) {
                memory[index_reg + i] = V[i];
            }
            break;
        // Read register values from index register starting at location I
        case 0x65:
            for (int i = 0; i < x; i++) {
                V[i] = memory[index_reg + i];
            }
            break;
        }
        break;
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
