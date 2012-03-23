CC = g++
CXXFLAGS = -Wall -g -std=c++0x -pthread
LDFLAGS = -Il 
BIN =pferd

OBJDIR=obj

# zu jeder X.cpp datei, die kompiliert wird, hier in
# OBJ eine entsprechende X.o file anlegen lassen
CPP =pferd.cpp graphalgs.cpp parser.cpp graph.cpp ch.cpp rlparser.cpp
OBJ =pferd.o graphalgs.o parser.o graph.o ch.o rlparser.o

all: $(BIN)

rlparser.cpp: rlparser.rl
	ragel -G2 -o $@ $<

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

# es werden all .cpp dateien kompiliert
$(OBJ): $(wildcard *.h) $(wildcard *.hpp)

clean:
	rm -f $(BIN) $(OBJ)

#fucking...
.PHONY: all clean
