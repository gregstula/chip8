# chip8
A multithreaded chip 8 emulator written in C++17 with SDL

![space-invaders](https://github.com/gregstula/chip8/raw/master/screenshot.png)


## Compiling
Requires: SDL2, Qt6

Install and play space invaders:
```
git clone https://github.com/gregstula/cmake
make cmake && make test
```

Manually using cmake
```
cmake -S . -B build
cmake --build build
```

## How to use with other ROMS
```
.build/chip8 [rom]
```

## License
Please see LICENSE file.
