#ifndef CHIP8
#define CHIP8

#include "Display.hpp"
#include <SDL2/SDL.h>
#include <string>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdio>

class Chip8 {
private:
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;

    uint16_t stack[16];
    int stackpointer;

    int delay_timer;
    int sound_timer;

    bool halted;

    Display *display;
    bool drawflag;

public:
    Chip8();
    ~Chip8();
    bool run();
    void events();
    void render();
    bool needsRedraw();
    void clearDrawflag();
    void loadFile(std::string filename);
    void loadFontset();
};

#endif