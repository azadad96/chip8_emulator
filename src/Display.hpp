#ifndef DISPLAY
#define DISPLAY

#include <SDL2/SDL.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>

class Display {
private:
    static const int scale = 10;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Keycode keytoid[16];

    bool key_was_pressed;

public:
    static const int width = 64, height = 32;
    uint8_t display[width * height];
    bool keys[16];
    bool running;

    Display();
    ~Display();
    void render();
    void events();
    bool keyPressed();
    void clearKeyFlag();
};

#endif