CC= clang++
PROG= ./bin/chiplus8
OBJS= ./src/main.o ./src/Chip8.o ./src/EmuCPU.o ./src/SimpleLogger/simplog.o
LIBS= 
CXXFLAGS= -g -Wall -std=c++11 $(shell pkg-config --cflags ${LIBS})
LDFLAGS= $(shell pkg-config --static --libs ${LIBS})

all: $(PROG)

$(PROG): $(OBJS)
	mkdir -p ./bin/
	$(CC) -o $(PROG) $(LDFLAGS) $(CFLAGS) $(OBJS)
	rm -f $(OBJS)

clean:
	rm -f $(PROG) $(TEST_PROG) $(OBJS)
