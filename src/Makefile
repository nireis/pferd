CC = g++
CXXFLAGS = -Wall -g -std=c++0x -lglut -lGL -pthread
LDFLAGS = -Il /usr/lib/libGLEW.so /usr/lib/libSOIL.a
BIN =pferd

OBJDIR=obj

# zu jeder X.cpp datei, die kompiliert wird, hier in
# OBJ eine entsprechende X.o file anlegen lassen
CPP =pferd.cpp graphalgs.cpp parser.cpp graph.cpp ch.cpp rlparser.cpp openGLrender.cpp vis.cpp chdijkstra.cpp clust.cpp sim.cpp dijkstra.cpp conf.cpp travel.cpp
OBJ =pferd.o graphalgs.o parser.o graph.o ch.o rlparser.o openGLrender.o vis.o chdijkstra.o clust.o sim.o dijkstra.o conf.o travel.o

all: $(BIN)

rlparser.cpp: rlparser.rl
	ragel -G2 -o $@ $<

sparser: sparser.cpp sparser.h
	g++ -o sparser -O3 sparser.cpp

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

# es werden all .cpp dateien kompiliert
$(OBJ): $(wildcard *.h) $(wildcard *.hpp)

clean:
	rm -f $(BIN) $(OBJ)

#fucking...
.PHONY: all clean
