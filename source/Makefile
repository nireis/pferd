CC = g++
CXXFLAGS = -Wall -g -O3
LDFLAGS = 
BIN =pferd

OBJDIR=obj

# zu jeder X.cpp datei, die kompiliert wird, hier in
# OBJ eine entsprechende X.o file anlegen lassen
CPP =pferd.cpp graphalgs.cpp parser.cpp graph.cpp ch.cpp
OBJ =pferd.o graphalgs.o parser.o graph.o ch.o

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

# es werden all .cpp dateien kompiliert
$(OBJ): $(wildcard *.h) $(wildcard *.hpp)

clean:
	rm -f $(BIN) $(OBJ)

.PHONY: all clean

#all: pferd
#
#pferd: pferd.o
#	$(CC) -o $@ $^
#
#clean:
#	rm -f pferd *.o
#
#.PHONY: all clean
#
