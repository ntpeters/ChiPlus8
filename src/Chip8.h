#pragma once

#include "EmuCPU.h"

class Chip8 {
public:
    Chip8( const char* filename );
    ~Chip8();
    void runProgram();

private:
    EmuCPU* cpu;
};