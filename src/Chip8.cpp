#include <fstream>

#include "Chip8.h"
#include "EmuCPU.h"
#include "./SimpleLogger/simplog.h"

Chip8::Chip8( const char* filename ) {
	writeLog( LOG_DEBUG, "Emulator created!" );

	cpu = new EmuCPU();

	cpu->loadProgram( filename );
}

Chip8::~Chip8() {
	writeLog( LOG_DEBUG, "Emulator destroyed!" );
}

void Chip8::runProgram() {
	writeLog( LOG_DEBUG, "Running program!" );

	bool done = false;

	while( !done ) {
		cpu->doCycle();
		this->drawScreen( cpu->getDisplay() );

		done = true;
	}
}

void Chip8::drawScreen( char** screen ) {
	writeLog( LOG_DEBUG, "Drawing the screen!" );
}