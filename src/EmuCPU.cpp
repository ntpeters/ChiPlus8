#include <iostream>
#include <fstream>

#include "EmuCPU.h"
#include "./SimpleLogger/simplog.h"

EmuCPU::EmuCPU() {
	writeLog( LOG_DEBUG, "CPU created!" );

	opcode = 0x000;
	I = 0;
	PC = 0x200;
}

EmuCPU::~EmuCPU() {
	writeLog( LOG_DEBUG, "CPU destroyed!" );
}

void EmuCPU::doCycle() {
	writeLog( LOG_DEBUG, "Cycle!" );

	this->fetch();
	this->decode_execute();
}

char** EmuCPU::getDisplay() {
	writeLog( LOG_DEBUG, "Getting display!" );

	return NULL;
}

void EmuCPU::loadProgram( const char* filename) {
	writeLog( LOG_DEBUG, "Loading program!" );
	writeLog( LOG_VERBOSE, "%s", filename );

	std::ifstream programFile;
	programFile.open( filename, std::ios::in | std::ios::binary | std::ios::ate );

	if( !programFile.is_open() ) {
		writeLog( LOG_FATAL, "Failed to open program file!" );
		throw -1;
	}

	int programSize = programFile.tellg();
	printf( "%d\n", programSize );
	if( programSize > 4096 - 512 ) {
		writeLog( LOG_FATAL, "Program too large to fit into memory!" );
		programFile.close();
		throw -1;
	}

	programFile.seekg( 0, std::ios::beg );
	programFile.read( (char*)memory + 512, programSize );
	programFile.close();
}

void EmuCPU::fetch() {
	writeLog( LOG_DEBUG, "Fetching opcode!" );

	opcode = memory[PC] << 8 | memory[PC + 1];

	writeLog( LOG_DEBUG, "Fetched Opcode: %A", opcode );
}

void EmuCPU::decode_execute() {
	writeLog( LOG_DEBUG, "Decoding and Executing opcode!" );

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
					for( int i = 0; i < screen_width * screen_height; i++ ) {
						gfx[i] = 0;
					}
					break;
				case 0x00EE:
					// return from a subroutine
					break;
				default:
					// 0x0NNN
					// calls program at address NNN
			}
			break;
		case 0x1000:
			// jump to address NNN
			break;
		case 0x2000:
			// call subroutine at NNN
			break;
		case 0x3000:
			// skip the next instruction if VX equals NN
			int X = opcode & 0x0F00;
			int NN = opcode & 0x00FF;

			if( V[X] == NN ) {
				PC += 2;
			}

			PC += 2;
			break;
		case 0x4000:
			// skip the next instruction if VX does not equal NN
			int X = opcode & 0x0F00;
			int NN = opcode & 0x00FF;

			if( V[X] != NN ) {
				PC += 2;
			}

			PC += 2;
			break;
		case 0x5000:
			// skips the next instruction iv VX equals VY
			break;
		case 0x6000:
			// set VX to NN
			break;
		case 0x7000:
			// add NN to VX
			break;
		case 0x8000:
			switch( opcode & 0x000F ) {
				case 0x0000:
					// set VX to the value of VY
					break;
				case 0x0001:
					// set VX to VX or VY
					break;
				case 0x0002:
					// set VX to VX and VY
					break;
				case 0x0003:
					// set VX to VX xor VY
					break;
				case 0x0004:
					// add VY to VX. VF is set to 1 when there's a carry, and 0 otherwise
					break;
				case 0x0005:
					// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 otherwise
					break;
				case 0x0006:
					// shift VX right by one. VF is set to the value of the lease significant bit before shift
					break;
				case 0x0007:
					// set VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 otherwise
					break;
				case 0x000E:
					// shift VX left by one. VF is set to the value of the most significant bit of VX before shift
					break;
				default:
					// Unknown opcode
			}
			break;
		case 0x9000:
			// skip the next instruction if VX does not equal VY
			break;
		case 0xA000:
			// set I to the address of NNN
			break;
		case 0xB000:
			// jump to the address NNN plus V0
			break;
		case 0xC000:
			// set VX to a random number and NN
		case 0xD000:
			// draws a sprite at coordinate(VX, VY) that has a width of 8 pixels
			//	and a height of N pixels.
			break;
		case 0xE000:
			switch( opcode & 0x00FF ) {
				case 0x009E:
					// skip the next instruction if the key in VX is pressed
					break;
				case 0x00A1:
					// skip the next instruction if the key stored in VX is not pressed
					break;
				default:
					// Unkown opcode
			}
			break;
		case 0xF000;
			switch( opcode & 0x00FF ) {
				case 0x0007:
					// set VX to the value of the delay timer
					break;
				case 0x000A:
					// wait for keypress and store it in VX
					break;
				case 0x0015;
					// set the delay timer to VX
					break;
				case 0x0018:
					// set the sound timer to VX
					break;
				case 0x001E:
					// add VX to I
					break;
				case 0x0029:
					// set I to the location of the sprite for the character in VX
					break;
				case 0x0033;
					// store the binary-coded-decimal representation of VX in I to I + 2
					//  take decimal representation of VX: place the hundreds digit in memory
					//  at location in I, the tens digit in I + 1, and the ones digit in I + 2
					break;
				case 0x0055:
					// store V0 to VX in memory starting at address I
					break;
				case 0x0065:
					// fill V0 to VX with values from memory starting at address I
					break;
				default:
					// Unknown opcode
			}
			break;
		default:
			// Unknown opcode
		}
	}
}