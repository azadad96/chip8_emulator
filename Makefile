main: bin/main.o bin/Chip8.o bin/Display.o
	g++ bin/*.o -o main -lSDL2

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
	rm main
	rm -rf bin