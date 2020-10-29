#include "Display.hpp"

Display::Display() {
    memset(display, 0, sizeof(display));
    running = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("Couldn't initialize SDL\n");
        exit(-1);
    }

    window = SDL_CreateWindow(
        "Chip8 emulator",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width * scale, height * scale,
        SDL_WINDOW_SHOWN
    );
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    key_was_pressed = false;

    memset(keys, false, sizeof(keys));
    keytoid[0] = SDLK_x;
    keytoid[1] = SDLK_1;
    keytoid[2] = SDLK_2;
    keytoid[3] = SDLK_3;
    keytoid[4] = SDLK_q;
    keytoid[5] = SDLK_w;
    keytoid[6] = SDLK_e;
    keytoid[7] = SDLK_a;
    keytoid[8] = SDLK_s;
    keytoid[9] = SDLK_d;
    keytoid[0xA] = SDLK_y;
    keytoid[0xB] = SDLK_c;
    keytoid[0xC] = SDLK_4;
    keytoid[0xD] = SDLK_r;
    keytoid[0xE] = SDLK_f;
    keytoid[0xF] = SDLK_v;
}

Display::~Display() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Display::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < width * height; i++) {
        int x = i % width * scale;
        int y = i / width * scale;
        if (display[i] != 0) {
            SDL_Rect rect = {x, y, scale, scale};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer);
}

void Display::events() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN: {
                SDL_Keycode sym = e.key.keysym.sym;
                for (int i = 0; i < 16; i++) {
                    if (keytoid[i] == sym)
                        keys[i] = true;
                }
                key_was_pressed = true;
                break;
            }

            case SDL_KEYUP: {
                SDL_Keycode sym = e.key.keysym.sym;
                for (int i = 0; i < 16; i++) {
                    if (keytoid[i] == sym)
                        keys[i] = false;
                }
                break;
            }
        }
    }
}

bool Display::keyPressed() {
    return key_was_pressed;
}

void Display::clearKeyFlag() {
    key_was_pressed = false;
}