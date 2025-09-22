CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++17
LIBS=-lSDL2 -pthread
SRC=main.cpp chip8.cpp display.cpp

all:
	$(CXX) $(CXXFLAGS) $(LIBS) -o chip8 $(SRC)

debug:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build

cmake:
	cmake -S . -B build && cmake --build build

test:
	build/chip8 ./roms/INVADERS

clean:
	@rm -f chip8
	@rm -f *.o
	@rm -rf build
format:
	clang-format --verbose --sort-includes -i *.cpp *.hpp

