#include "Chip8.h"
#include "./SimpleLogger/simplog.h"

const char* version = "Development Build";

// Logger settings
bool keepLogs       = false;
const char* logFile = "chip8.log";
int logLevel        = LOG_VERBOSE;
bool silent         = false;

const char* programFileName = "Chip-8 Pack/Chip-8 Games/Space Invaders [David Winter].ch8";
//const char* programFileName = "testLargeFile.txt";

// Prototypes
void initLogger();

int main() {
	initLogger();

	simplog.writeLog( LOG_INFO, "Starting ChiPlus8, Chip 8 Emulator - Version: %s", version );

	try {
	Chip8* emulator = new Chip8( programFileName );

	emulator->runProgram();

	} catch( int error ) {
		// Throw up error dialog

		return error;
	}

	return 0;
}

void initLogger() {
    // Set logfile name
    simplog.setLogFile( logFile );
    
    // Suppresses log output to stdout when true
    simplog.setLogSilentMode( silent );

    // Flush the logfile
    if( !keepLogs ) {
        simplog.flushLog();
    }

    // Set the level of logger output
    simplog.setLogDebugLevel( logLevel );
}