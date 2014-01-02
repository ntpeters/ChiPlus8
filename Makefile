CC= clang++
PROG= ./bin/chiplus8
OBJS= ./src/main.o ./src/Chip8.o ./src/EmuCPU.o ./src/SimpleLogger/simplog.o
LIBS= 
CXXFLAGS= -g -Wall -std=c++11 $(shell pkg-config --cflags ${LIBS})
LDFLAGS= $(shell pkg-config --static --libs ${LIBS})

all: logger $(PROG)

$(PROG): $(OBJS)
	mkdir -p ./bin/
	$(CC) -rdynamic -o $(PROG) $(LDFLAGS) $(OBJS)
	rm -f $(OBJS)

logger:
	cd ./src/SimpleLogger; git pull origin master; make clean all

clean:
	rm -f $(PROG) $(OBJS)
