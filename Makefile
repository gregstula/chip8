CXX=g++
CXXFLAGS=-Wall -std=c++17
LIBS=-lSDL2
SRC=main.cpp chip8.cpp display.cpp

all:
	$(CXX) $(CXXFLAGS) $(LIBS) -o chip8 $(SRC)
debug:
	g++ -g $(CXXFLAGS) -DMESSAGE='"Compiled with GCC"' $(LIBS) -o chip8 $(SRC)

clean:
	@rm -f chip8
	@rm -f *.o
format:
	clang-format --verbose --sort-includes -i *.cpp *.hpp
