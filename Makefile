all: bin/main.o bin/Chip8.o bin/Display.o
	g++ bin/*.o -o chip8emu -lSDL2

install:
	cp chip8emu /usr/bin/chip8emu

uninstall:
	rm /usr/bin/chip8emu

bin/main.o: src/main.cpp
	mkdir -p bin
	g++ -c src/main.cpp -o bin/main.o

bin/Chip8.o: src/Chip8.hpp src/Chip8.cpp
	mkdir -p bin
	g++ -c src/Chip8.cpp -o bin/Chip8.o

bin/Display.o: src/Display.hpp src/Display.cpp
	mkdir -p bin
	g++ -c src/Display.cpp -o bin/Display.o

clean:
	rm chip8emu
	rm -rf bin
