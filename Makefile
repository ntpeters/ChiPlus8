CC= clang++
PROG= ./bin/chiplus8
OBJS= ./src/main.o ./src/Chip8.o ./src/EmuCPU.o
LIBS= 
CXXFLAGS= -g -Wall -std=c++11 $(shell pkg-config --cflags ${LIBS})
LDFLAGS= $(shell pkg-config --static --libs ${LIBS})

all: $(PROG)

$(PROG): $(OBJS)
	mkdir -p ./bin/
	$(CC) -g -rdynamic -o $(PROG) $(LDFLAGS) $(OBJS) -L src/SimpleLogger/ -lsimplog
	rm -f $(OBJS)

logger:
	cd ./src/SimpleLogger; make clean all

clean:
	rm -f $(PROG) $(OBJS)
