#include <fstream>

#include "Chip8.h"
#include "EmuCPU.h"
#include "./SimpleLogger/simplog.h"

Chip8::Chip8( const char* filename ) {
	simplog.writeLog( SIMPLOG_DEBUG, "Emulator created!" );

	cpu = new EmuCPU();

	cpu->loadProgram( filename );
}

Chip8::~Chip8() {
	simplog.writeLog( SIMPLOG_DEBUG, "Emulator destroyed!" );
}

void Chip8::runProgram() {
	simplog.writeLog( SIMPLOG_DEBUG, "Running program!" );

	bool run = true;
	while( run ) {
		run = cpu->doCycle();
	}
}