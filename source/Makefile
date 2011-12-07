CC = g++
CXXFLAGS = -Wall -g -O3
LDFLAGS = 
BIN =pferd

# zu jeder X.cpp datei, die kompiliert wird, hier in
# OBJ eine entsprechende X.o file anlegen lassen
CPP =pferd.cpp graphalgs.cpp parser.cpp simple_parser.cpp
OBJ =pferd.o graphalgs.o parser.o simple_parser.o

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
