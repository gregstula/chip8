cmake:
	cmake -S . -B build && cmake --build build

debug:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build

test:
	build/chip8 ./roms/INVADERS

clean:
	@rm -f chip8
	@rm -f *.o
	@rm -rf build
format:
	clang-format --verbose --sort-includes -i *.cpp *.hpp

