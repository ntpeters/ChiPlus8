#include <iostream>
#include <fstream>

#include "EmuCPU.h"
#include "./SimpleLogger/simplog.h"

EmuCPU::EmuCPU() {
	simplog.writeLog( SIMPLOG_DEBUG, "CPU created!" );

	opcode = 0x000;
	I = 0;
	PC = 0x200;
	drawFlag = false;
	programMemoryBoundry = 0;
}

EmuCPU::~EmuCPU() {
	simplog.writeLog( SIMPLOG_DEBUG, "CPU destroyed!" );
}

bool EmuCPU::doCycle() {
	simplog.writeLog( SIMPLOG_DEBUG, "Cycle!" );

	bool fetch_OK = false;
	bool decode_execute_OK = false;

	fetch_OK = this->fetch();
	if( fetch_OK ) { 
		decode_execute_OK = this->decode_execute();
	}

	if( drawFlag ) {
		this->drawScreen();
	}

    simplog.writeLog( SIMPLOG_WARN, "Test warning." );
    simplog.writeLog( SIMPLOG_ERROR, "Test error." );

	return decode_execute_OK;
}

void EmuCPU::loadProgram( const char* filename) {
	simplog.writeLog( SIMPLOG_DEBUG, "Loading program!" );
	simplog.writeLog( SIMPLOG_VERBOSE, "%s", filename );

	std::ifstream programFile;
	programFile.open( filename, std::ios::in | std::ios::binary | std::ios::ate );

	if( !programFile.is_open() ) {
		simplog.writeLog( SIMPLOG_FATAL, "Failed to open program file!" );
		throw -1;
	}

	int programSize = programFile.tellg();
	simplog.writeLog( SIMPLOG_VERBOSE, "Program Size: %d bytes", programSize );
	if( programSize > 4096 - 512 ) {
		simplog.writeLog( SIMPLOG_FATAL, "Program too large to fit into memory!" );
		programFile.close();
		throw -1;
	}

	programFile.seekg( 0, std::ios::beg );
	programFile.read( (char*)memory + 512, programSize );

	if( programFile ) {
		simplog.writeLog( SIMPLOG_INFO, "Program loaded successfully!" );
	} else {
		simplog.writeLog( SIMPLOG_ERROR, "Error reading program!" );
	}

	this->programMemoryBoundry = programSize + PC;

	programFile.close();
}

bool EmuCPU::fetch() {
	simplog.writeLog( SIMPLOG_DEBUG, "Fetching opcode!" );

	if( PC < 0xFFF && PC < programMemoryBoundry ) {
		// Fetch current opcode from memory
		// Opcode is two bytes long, so we need to OR the next two bytes together
		opcode = memory[PC] << 8 | memory[PC + 1];
		simplog.writeLog( SIMPLOG_DEBUG, "Fetched Opcode: %X", opcode );
		return true;
	} else {
		// The program should be complete. We don't want to read past the program space in memory
		simplog.writeLog( SIMPLOG_DEBUG, "Program complete. PC exeeded program space in memory." );
		return false;
	}
}

bool EmuCPU::decode_execute() {
	simplog.writeLog( SIMPLOG_DEBUG, "Decoding and Executing opcode!" );

	// decode

	/*
	Key:
	NNN = address
	NN = 8bit constant
	N = 4bit constant
	X and Y = 4 bit register identifier
	*/
	switch( opcode & 0xF000 ) {
		case 0x0000:
			switch( opcode & 0x00FF ) {
				case 0x00E0:
					// clear the screen
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x00E0" );
					for( unsigned int i = 0; i < screen_width * screen_height; i++ ) {
						gfx[i] = 0;
					}

					PC += 2;
					break;
				case 0x00EE:
					// return from a subroutine
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x00EE" );
					PC = stack[sp];
					stack[sp] = 0;
					sp--;
					break;
				default:
					// 0x0NNN
					// calls program at address NNN
					// TODO
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x0NNN" );
			}
			break;
		case 0x1000:
			// jump to address NNN
			simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x1NNN" );
			PC = opcode & 0x0FFF;
			break;
		case 0x2000:
			// call subroutine at NNN
			simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x2NNN" );
			stack[sp] = PC;
			sp++;
			PC = opcode & 0x0FFF;
			break;
		case 0x3000:
			// skip the next instruction if VX equals NN
			simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x3XNN" );
			if( V[( opcode & 0x0F00 ) >> 8] == ( opcode & 0x00FF ) ) {
				PC += 2;
			}

			PC += 2;
			break;
		case 0x4000:
			// skip the next instruction if VX does not equal NN
			simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x4XNN" );
			if( V[( opcode & 0x0F00 ) >> 8] != ( opcode & 0x00FF ) ) {
				PC += 2;
			}

			PC += 2;
			break;
		case 0x5000:
			// skips the next instruction if VX equals VY
			simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x5XY0" );
			if( V[( opcode & 0x0F00 ) >> 8] == V[( opcode & 0x00F0 ) >> 4] ) {
				PC += 2;
			}

			PC += 2;
			break;
		case 0x6000:
			// set VX to NN
			simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x6XNN" );
			V[( opcode & 0x0F00 ) >> 8] = ( opcode & 0x0F00 ) >> 8;

			PC += 2;
			break;
		case 0x7000:
			// add NN to VX
			simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x7XNN" );
			V[( opcode & 0x0F00 ) >> 8] += opcode & 0x00FF;

			PC += 2;
 			break;
		case 0x8000:
			switch( opcode & 0x000F ) {
				case 0x0000:
					// set VX to the value of VY
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x8XY0" );
					V[( opcode & 0x0F00 ) >> 8] = V[( opcode & 0x00F0 ) >> 4];

					PC += 2;
					break;
				case 0x0001:
					// set VX to VX or VY
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x8XY1" );
					V[( opcode & 0x0F00 ) >> 8] = V[( opcode & 0x0F00 ) >> 8] | V[( opcode & 0x00F0 ) >> 4];

					PC += 2;
					break;
				case 0x0002:
					// set VX to VX and VY
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x8XY2" );
					V[( opcode & 0x0F00 ) >> 8] = V[( opcode & 0x0F00 ) >> 8] & V[( opcode & 0x00F0 ) >> 4];

					PC += 2;
					break;
				case 0x0003:
					// set VX to VX xor VY
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x8XY3" );
					V[( opcode & 0x0F00 ) >> 8] = V[( opcode & 0x0F00 ) >> 8] ^ V[( opcode & 0x00F0 ) >> 4];
					
					PC += 2;
					break;
				case 0x0004:
					// add VY to VX. VF is set to 1 when there's a carry, and 0 otherwise
					// TODO
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x8XY4" );
					if( V[ ( opcode & 0x00F0 ) >> 4 ] > ( 0xFF - V[ ( opcode & 0x0F00 ) >> 8 ] ) ) {
    					V[0xF] = 1; //carry
					} else {
    					V[0xF] = 0;
  					}
  					V[ ( opcode & 0x0F00 ) >> 8 ] += V[ ( opcode & 0x00F0 ) >> 4 ];
  					
  					PC += 2;
					break;
				case 0x0005:
					// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 otherwise
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x8XY5" );

					PC += 2;
					break;
				case 0x0006:
					// shift VX right by one. VF is set to the value of the lease significant bit before shift
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x8XY6" );
					if( ( opcode & 0x0F00 ) & 0x0001 ) {
						V[0xF] = 1;
					} else {
						V[0xF] = 0;
					}
					V[opcode & 0x0F00] >>= 1;

					PC += 2;
					break;
				case 0x0007:
					// set VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 otherwise
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x8XY7" );

					PC += 2;
					break;
				case 0x000E:
					// shift VX left by one. VF is set to the value of the most significant bit of VX before shift
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x8XYE" );
					if( ( opcode & 0x0F00 ) & 0x1000 ) {
						V[0xF] = 1;
					} else {
						V[0xF] = 0;
					}
					V[opcode & 0x0F00] <<= 1;

					PC += 2;
					break;
				default:
					// Unknown opcode
					simplog.writeLog( SIMPLOG_VERBOSE, "Unknown Opcode!" );
			}
			break;
		case 0x9000:
			// skip the next instruction if VX does not equal VY
			simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0x9XY0" );
			if( V[( opcode & 0x0F00 ) >> 8] != V[( opcode & 0x00F0 ) >> 4] ) {
				PC += 4;
			} else { 
				PC += 2;
			}

			break;
		case 0xA000:
			// set I to the address of NNN
			simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xANNN" );
			I = opcode & 0x0FFF;
			PC += 2;
			break;
		case 0xB000:
			// jump to the address NNN plus V0
			// TODO
			simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xBNNN" );

			PC += 2;
			break;
		case 0xC000:
			// set VX to a random number and NN
			// TODO
			simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xCXNN" );

			PC += 2;
		case 0xD000:
			// draws a sprite at coordinate(VX, VY) that has a width of 8 pixels
			//	and a height of N pixels.
			// TODO
			simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xDXYN" );



			PC += 2;
			break;
		case 0xE000:
			switch( opcode & 0x00FF ) {
				case 0x009E:
					// skip the next instruction if the key in VX is pressed
					// TODO
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xEX9E" );

					PC += 2;
					break;
				case 0x00A1:
					// skip the next instruction if the key stored in VX is not pressed
					// TODO
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xEXA1" );

					PC += 2;
					break;
				default:
					// Unkown opcode
					simplog.writeLog( SIMPLOG_VERBOSE, "Unknown Opcode!" );
			}
			break;
		case 0xF000:
			switch( opcode & 0x00FF ) {
				case 0x0007:
					// set VX to the value of the delay timer
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xFX07" );
					V[( opcode & 0x0F00 ) >> 8] = delay_timer;

					PC += 2;
					break;
				case 0x000A:
					// wait for keypress and store it in VX
					// TODO
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xFX0A" );

					PC += 2;
					break;
				case 0x0015:
					// set the delay timer to VX
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xFX15" );
					delay_timer = V[( opcode & 0x0F00 ) >> 8];

					PC += 2;
					break;
				case 0x0018:
					// set the sound timer to VX
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xFX18" );
					sound_timer = V[( opcode & 0x0F00 ) >> 8];

					PC += 2;
					break;
				case 0x001E:
					// add VX to I
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xFX1E" );
					I += V[( opcode & 0x0F00 ) >> 8];

					PC += 2;
					break;
				case 0x0029:
					// set I to the location of the sprite for the character in VX
					// TODO
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xFX29" );

					PC += 2;
					break;
				case 0x0033:
					// store the binary-coded-decimal representation of VX in I to I + 2
					//  take decimal representation of VX: place the hundreds digit in memory
					//  at location in I, the tens digit in I + 1, and the ones digit in I + 2
					// TODO
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xFX33" );
					memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
					memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
					memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
					
					PC += 2;
					break;
				case 0x0055:
					// store V0 to VX in memory starting at address I
					// TODO
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xFX55" );

					PC += 2;
					break;
				case 0x0065:
					// fill V0 to VX with values from memory starting at address I
					// TODO
					simplog.writeLog( SIMPLOG_VERBOSE, "Executing: 0xFX65" );

					PC += 2;
					break;
				default:
					// Unknown opcode
					simplog.writeLog( SIMPLOG_VERBOSE, "Unknown Opcode!" );
			}
			break;
		default:
			// Unknown opcode
			simplog.writeLog( SIMPLOG_VERBOSE, "Unknown Opcode!" );
		}

		// update the timers
		if( delay_timer > 0 ) {
			delay_timer--;
		}

		if( sound_timer > 0 ) {
			if( sound_timer == 1 ) {
				// Play beep sound
				simplog.writeLog( SIMPLOG_VERBOSE, "BEEP!" );
			}

			sound_timer--;
		}

		simplog.writeStackTrace();
		throw -1;
		
		return true;
	}

	void EmuCPU::drawScreen() {
		simplog.writeLog( SIMPLOG_VERBOSE, "Draw screen..." );

		drawFlag = false;
	}
