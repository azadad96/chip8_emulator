#include "Chip8.hpp"
#include <thread>
#include <chrono>

int main(int argc, char **argv) {
    Chip8 *chip = new Chip8();
    chip->loadFile(argv[1]);

    bool running = true;
    while (running) {
        auto start = std::chrono::high_resolution_clock::now();

        chip->events();
        running = chip->run();
        if (chip->needsRedraw()) {
            chip->render();
            chip->clearDrawflag();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 180 - diff.count()));
    }

    delete chip;
    return 0;
}