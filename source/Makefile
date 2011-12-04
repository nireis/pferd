CC = g++
CFLAGS = -Wall -g
LDFLAGS = 
BIN =pferd

# zu jeder X.cpp datei, die kompiliert wird, hier in
# OBJ eine entsprechende X.o file anlegen lassen
CPP =pferd.cpp graphalgs.cpp parser.cpp simple_parser.cpp
OBJ =pferd.o graphalgs.o parser.o simple_parser.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

# es werden all .cpp dateien kompiliert
$(OBJ):
	$(CC) $(CFLAGS) -c $(CPP)

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
