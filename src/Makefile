CC = g++
CXXFLAGS = -Wall -g -std=c++0x -lglut -lGL -pthread -O3
LDFLAGS = -Il /usr/lib/x86_64-linux-gnu/libGLEW.so /usr/lib/libSOIL.a
#LDFLAGS = -Il /usr/lib/libGLEW.so /usr/lib/libSOIL.a

BIN =pferd

OBJDIR=obj

# zu jeder X.cpp datei, die kompiliert wird, hier in
# OBJ eine entsprechende X.o file anlegen lassen
#CPP =pferd.cpp graphalgs.cpp parser.cpp graph.cpp ch.cpp rlparser.cpp openGLrender.cpp vis.cpp chdijkstra.cpp clust.cpp sim.cpp dijkstra.cpp conf.cpp travel.cpp tconfreader.cpp
OBJ = graphalgs.o parser.o graph.o ch.o rlparser.o openGLrender.o vis.o chdijkstra.o clust.o sim.o dijkstra.o conf.o travel.o tconfreader.o


all: $(BIN)

bench: $(OBJ) bench.o
	$(CC) -o benchmark bench.o $(OBJ) $(LDFLAGS) $(CXXFLAGS) 
	mkdir -p ../benchdata

rlparser.cpp: rlparser.rl
	ragel -G2 -o $@ $<

sparser: sparser.cpp sparser.h
	g++ -o sparser -O3 sparser.cpp

$(BIN): $(OBJ) pferd.o
	$(CC) -o $(BIN) pferd.o $(OBJ) $(LDFLAGS) $(CXXFLAGS) 

bench.o: bench.cpp
	$(CC) $(CXXFLAGS)   -c -o bench.o bench.cpp 

# es werden all .cpp dateien kompiliert
$(OBJ): $(wildcard *.h) $(wildcard *.hpp)


clean:
	rm -f $(BIN) $(OBJ) pferd.o bench.o benchmark

#fucking...
.PHONY: all clean
