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
	void doCycle();
	char** getDisplay();
    void loadProgram( const char* filename );

private:
	void fetch();
	void decode_execute();

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

    // Keyboard support. Keys: 0x00 - 0x0F
    unsigned char key[16] = {0};
};