CC = g++
CXXFLAGS = -Wall -g -std=c++0x -lmarblewidget -lQtGui -O3
CFLAGS = -Wall -g -O3
LDFLAGS = -Il /usr/lib/libQtCore.so.4
BIN =pferd

OBJDIR=obj

# zu jeder X.cpp datei, die kompiliert wird, hier in
# OBJ eine entsprechende X.o file anlegen lassen
CPP =pferd.cpp graphalgs.cpp parser.cpp graph.cpp ch.cpp rlparser.c vis.cpp
OBJ =pferd.o graphalgs.o parser.o graph.o ch.o rlparser.o vis.o

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

# es werden all .cpp dateien kompiliert
$(OBJ): $(wildcard *.h) $(wildcard *.hpp)

clean:
	rm -f $(BIN) $(OBJ)

#fucking...
.PHONY: all clean
