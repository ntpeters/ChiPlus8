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

	
}

void EmuCPU::fetch() {
	writeLog( LOG_DEBUG, "Fetching opcode!" );

	opcode = memory[PC] << 8 | memory[PC + 1];

	writeLog( LOG_DEBUG, "Fetched Opcode: %A", opcode );
}

void EmuCPU::decode_execute() {
	writeLog( LOG_DEBUG, "Decoding and Executing opcode!" );


}