#include "Chip8.hpp"

Chip8::Chip8() {
    memset(memory, 0, sizeof(memory));
    memset(V, 0, sizeof(V));
    I = 0;
    pc = 0x200;

    memset(stack, 0, sizeof(stack));
    stackpointer = 0;

    delay_timer = 0;
    sound_timer = 0;

    halted = false;

    display = new Display();
    drawflag = false;

    loadFontset();
}

Chip8::~Chip8() {
    delete display;
}

void Chip8::events() {
    display->events();
}

void Chip8::render() {
    display->render();
}

bool Chip8::needsRedraw() {
    return drawflag;
}

void Chip8::clearDrawflag() {
    drawflag = false;
}

bool Chip8::run() {
    if (halted) {
        if (display->keyPressed()) {
            display->clearKeyFlag();
            halted = false;
        }
        return display->running;
    }

    uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];

    switch (opcode & 0xF000) {
        case 0x0000: {
            switch (opcode & 0x00FF) {
                case 0x00E0: {
                    for (int i = 0; i < display->width * display->height; i++) {
                        display->display[i] = 0;
                    }
                    pc += 2;
                    break;
                }

                case 0x00EE: {
                    stackpointer--;
                    pc = stack[stackpointer] + 2;
                    break;
                }

                default: {
                    printf("Unsupported opcode\n");
                    exit(-1);
                    break;
                }
            }
            break;
        }

        case 0x1000: {
            int nnn = opcode & 0x0FFF;
            pc = nnn;
            break;
        }

        case 0x2000: {
            int nnn = opcode & 0x0FFF;
            stack[stackpointer] = pc;
            stackpointer++;
            pc = nnn;
            break;
        }

        case 0x3000: {
            int x = (opcode & 0x0F00) >> 8;
            int nn = opcode & 0x00FF;
            if (V[x] == nn)
                pc += 4;
            else
                pc += 2;
            break;
        }

        case 0x4000: {
            int x = (opcode & 0x0F00) >> 8;
            int nn = opcode & 0x00FF;
            if (V[x] != nn)
                pc += 4;
            else
                pc += 2;
            break;
        }

        case 0x5000: {
            int x = (opcode & 0x0F00) >> 8;
            int y = (opcode & 0x00F0) >> 4;
            if (V[x] == V[y])
                pc += 4;
            else
                pc += 2;
            break;
        }

        case 0x6000: {
            int x = (opcode & 0x0F00) >> 8;
            int nn = opcode & 0x00FF;
            V[x] = nn;
            pc += 2;
            break;
        }

        case 0x7000: {
            int x = (opcode & 0x0F00) >> 8;
            int nn = opcode & 0x00FF;
            V[x] += nn;
            pc += 2;
            break;
        }

        case 0x8000: {
            switch (opcode & 0x000F) {
                case 0x0000: {
                    int x = (opcode & 0x0F00) >> 8;
                    int y = (opcode & 0x00F0) >> 4;
                    V[x] = V[y];
                    pc += 2;
                    break;
                }

                case 0x0001: {
                    int x = (opcode & 0x0F00) >> 8;
                    int y = (opcode & 0x00F0) >> 4;
                    V[x] = V[x] | V[y];
                    pc += 2;
                    break;
                }

                case 0x0002: {
                    int x = (opcode & 0x0F00) >> 8;
                    int y = (opcode & 0x00F0) >> 4;
                    V[x] = V[x] & V[y];
                    pc += 2;
                    break;
                }

                case 0x0003: {
                    int x = (opcode & 0x0F00) >> 8;
                    int y = (opcode & 0x00F0) >> 4;
                    V[x] = V[x] ^ V[y];
                    pc += 2;
                    break;
                }

                case 0x0004: {
                    int x = (opcode & 0x0F00) >> 8;
                    int y = (opcode & 0x00F0) >> 4;
                    int vx = V[x], vy = V[y];
                    if (vx + vy > 255)
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;
                    V[x] = (vx + vy) % 256;
                    pc += 2;
                    break;
                }

                case 0x0005: {
                    int x = (opcode & 0x0F00) >> 8;
                    int y = (opcode & 0x00F0) >> 4;
                    int vx = V[x], vy = V[y];
                    if (vx > vy)
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;
                    V[x] = (vx - vy) & 0xFF;
                    pc += 2;
                    break;
                }

                case 0x0006: {
                    int x = (opcode & 0x0F00) >> 8;
                    V[0xF] = V[x] & 1;
                    V[x] >>= 1;
                    pc += 2;
                    break;
                }
                
                case 0x0007: {
                    int x = (opcode & 0x0F00) >> 8;
                    int y = (opcode & 0x00F0) >> 4;
                    int vx = V[x], vy = V[y];
                    if (vy > vx)
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;
                    V[x] = (vy - vx) & 0xFF;
                    pc += 2;
                    break;
                }

                case 0x000E: {
                    int x = (opcode & 0x0F00) >> 8;
                    V[0xF] = V[x] >> 7;
                    V[x] <<= 1;
                    pc += 2;
                    break;
                }

                default: {
                    printf("Unsupported opcode\n");
                    exit(-1);
                    break;
                }
            }
            break;
        }

        case 0x9000: {
            int x = (opcode & 0x0F00) >> 8;
            int y = (opcode & 0x00F0) >> 4;
            if (V[x] != V[y])
                pc += 4;
            else
                pc += 2;
            break;
        }

        case 0xA000: {
            int nnn = opcode & 0x0FFF;
            I = nnn;
            pc += 2;
            break;
        }

        case 0xA000: {
            int nnn = opcode & 0x0FFF;
            pc = nnn + V[0];
            break;
        }

        case 0xC000: {
            int x = (opcode & 0x0F00) >> 8;
            int nn = opcode & 0x00FF;
            V[x] = ((uint8_t) (rand() % 256)) & nn;
            pc += 2;
            break;
        }

        case 0xD000: {
            int x = (opcode & 0x0F00) >> 8;
            int y = (opcode & 0x00F0) >> 4;
            int n = opcode & 0x000F;

            int _x = V[x];
            int _y = V[y];

            V[0xF] = 0;
            for (int i = 0; i < n; i++) {
                uint8_t row = memory[I + i];
                for (int j = 0; j < 8; j++) {
                    uint8_t mask = 0b10000000 >> j;
                    int bit = (row & mask) != 0 ? 1 : 0;
                    int index = (i + _y) % 32 * display->width + (j + _x) % 64;
                    int before = display->display[index];
                    int after = before ^ bit;
                    display->display[index] = after;
                    if (before == 1 && before != after)
                        V[0xF] = 1;
                }
            }
            drawflag = true;
            pc += 2;
            break;
        }

        case 0xE000: {
            switch (opcode & 0x00FF) {
                case 0x009E: {
                    int x = (opcode & 0x0F00) >> 8;
                    if (display->keys[V[x]])
                        pc += 4;
                    else
                        pc += 2;
                    break;
                }

                case 0x00A1: {
                    int x = (opcode & 0x0F00) >> 8;
                    if (!display->keys[V[x]])
                        pc += 4;
                    else
                        pc += 2;
                    break;
                }

                default: {
                    printf("Unsupported opcode\n");
                    exit(-1);
                    break;
                }
            }
            break;
        }

        case 0xF000: {
            switch(opcode & 0x00FF) {
                case 0x0007: {
                    int x = (opcode & 0x0F00) >> 8;
                    V[x] = delay_timer;
                    pc += 2;
                    break;
                }

                case 0x000A: {
                    int x = (opcode & 0x0F00) >> 8;
                    display->clearKeyFlag();
                    halted = true;
                    pc += 2;
                    break;
                }

                case 0x0015: {
                    int x = (opcode & 0x0F00) >> 8;
                    delay_timer = V[x];
                    pc += 2;
                    break;
                }

                case 0x0018: {
                    int x = (opcode & 0x0F00) >> 8;
                    sound_timer = V[x];
                    pc += 2;
                    break;
                }

                case 0x001E: {
                    int x = (opcode & 0x0F00) >> 8;
                    I += V[x];
                    pc += 2;
                    break;
                }

                case 0x0029: {
                    int x = (opcode & 0x0F00) >> 8;
                    int character = V[x];
                    I = 0x50 + character * 5;
                    pc += 2;
                    break;
                }

                case 0x0033: {
                    int x = (opcode & 0x0F00) >> 8;
                    int value = V[x];
                    memory[I] = value / 100;
                    memory[I + 1] = (value % 100) / 10;
                    memory[I + 2] = value % 10;
                    pc += 2;
                    break;
                }

                case 0x0055: {
                    int x = (opcode & 0x0F00) >> 8;
                    for (int i = 0; i <= x; i++) {
                        memory[I] = V[i];
                        I++;
                    }
                    pc += 2;
                    break;
                }

                case 0x0065: {
                    int x = (opcode & 0x0F00) >> 8;
                    for (int i = 0; i <= x; i++) {
                        V[i] = memory[I];
                        I++;
                    }
                    pc += 2;
                    break;
                }

                default: {
                    printf("Unsupported opcode\n");
                    exit(-1);
                    break;
                }
            }
            break;
        }

        default: {
            printf("Unsupported opcode\n");
            exit(-1);
            break;
        }
    }

    if (delay_timer > 0)
        delay_timer--;
    if (sound_timer > 0)
        sound_timer--;

    return display->running;
}

void Chip8::loadFile(std::string filename) {
    FILE *f = fopen(filename.c_str(), "rb");
    if (!f) {
        printf("Couldn't read data from file\n");
        exit(-1);
    }

    uint8_t c;
    for (int i = 0x200; i < 4096 && !feof(f); i++) {
        c = fgetc(f);
        memory[i] = c;
    }

    fclose(f);
}

void Chip8::loadFontset() {
    uint8_t fontset[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xF0, 0x10, 0xF0, 0x80, 0xF0,
        0x90, 0x90, 0xF0, 0x10, 0x10,
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        0xF0, 0x80, 0xF0, 0x90, 0xF0,
        0xF0, 0x10, 0x20, 0x40, 0x40,
        0xF0, 0x90, 0xF0, 0x90, 0xF0,
        0xF0, 0x90, 0xF0, 0x10, 0xF0,
        0xF0, 0x90, 0xF0, 0x90, 0x90,
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xE0, 0x90, 0x90, 0x90, 0xE0,
        0xF0, 0x80, 0xF0, 0x80, 0xF0,
        0xF0, 0x80, 0xF0, 0x80, 0x80
    };

    for (int i = 0; i < sizeof(fontset) / sizeof(fontset[0]); i++) {
        memory[0x50 + i] = fontset[i];
    }
}