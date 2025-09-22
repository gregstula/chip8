#pragma once
#include "keypad.hpp"
#include <QApplication>
#include <QCloseEvent>
#include <QString>
#include <QTimer>
#include <QWidget>
#include <SDL2/SDL.h>
#include <array>
#include <cstdint>
#include <memory>
#include <string>

#include "chip8.hpp"

namespace sdl8 {

using chip8::SCREEN_DIMS;
using chip8::SCREEN_HEIGHT;
using chip8::SCREEN_WIDTH;

// scale factor
constexpr auto SCALE = 20;

class display {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

public:
    // draw function
    void render(std::array<uint8_t, SCREEN_DIMS>& data);

    display();
    display(void* window_handle);
    ~display() noexcept;

    display(const display&) = delete;
    display& operator=(const display&) = delete;
    display(display&&) = delete;
    display& operator=(display&&) = delete;
};
} // namespace sdl8

namespace qt8 {
using chip8::SCREEN_DIMS;
using chip8::SCREEN_HEIGHT;
using chip8::SCREEN_WIDTH;

// scale factor
using sdl8::SCALE;

class display_widget : public QWidget {
    Q_OBJECT

public:
    std::unique_ptr<sdl8::display> display;

    explicit display_widget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        setAttribute(Qt::WA_NativeWindow);
        setAttribute(Qt::WA_PaintOnScreen);
        setAttribute(Qt::WA_OpaquePaintEvent);
        setFixedSize(SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE);
    }

    void showEvent(QShowEvent* event) override
    {
        QWidget::showEvent(event);
        if (!display) {
            QTimer::singleShot(0, this, [this]() {
                this->display = std::make_unique<sdl8::display>();
            });
        }
    }

    void run_vm(std::string rom)
    {
        if (!display) return;

        chip8::vm vm {};
        vm.load_rom(rom);
        vm.start_timers();

        bool quit = false;
        while (!quit) {
            QApplication::processEvents();
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
            display->render(vm.screen);
        }
        QApplication::quit();
    }

protected:
    QPaintEngine*
    paintEngine() const override
    {
        return nullptr;
    }
};

} // namespace qt8
