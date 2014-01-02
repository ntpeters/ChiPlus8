#pragma once

/*
Chip 8 Memory Map:
0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
0x200-0xFFF - Program ROM and work RAM
*/

class EmuCPU {
public:
	EmuCPU();
	~EmuCPU();
	bool doCycle();
    void loadProgram( const char* filename );

private:
	bool fetch();
	bool decode_execute();
    void drawScreen();

    // Display dimensions constants
    static const unsigned int screen_width = 64;
    static const unsigned int screen_height = 32;

	// Used to store the current opcode
    unsigned short opcode;
    
    // 4k of program memory
    unsigned char memory[4096] = {0};
    
    // 16 general purpose registers
    unsigned char V[16] = {0};
    // Register index
    unsigned short I;

    // Program counter
    unsigned short PC;

    // Graphical Display
    unsigned char gfx[ screen_width * screen_height ] = {0};

    unsigned char delay_timer;

    // Buzzer sounds when zero
    unsigned char sound_timer;

    // Stack and stack pointer
    unsigned short stack[16];
    unsigned short sp;

    // Keyboard support. Keys: 0x00 - 0x0F
    unsigned char key[16] = {0};

    // Font set for Chip 8
    unsigned char chip8_fontset[80] =
    {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // Flag to denote if the screen should be redrawn
    bool drawFlag;

    // Program boundry in memory
    int programMemoryBoundry;
};