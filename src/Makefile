CC = g++
CXXFLAGS = -Wall -g -std=c++0x -lglut -lGL -pthread -O3
LDFLAGS = -Il /usr/lib/libGLEW.so /usr/lib/libSOIL.a
BIN =pferd

OBJDIR=obj

# zu jeder X.cpp datei, die kompiliert wird, hier in
# OBJ eine entsprechende X.o file anlegen lassen
CPP =pferd.cpp graphalgs.cpp parser.cpp graph.cpp ch.cpp rlparser.cpp openGLrender.cpp vis.cpp chdijkstra.cpp
OBJ =pferd.o graphalgs.o parser.o graph.o ch.o rlparser.o openGLrender.o vis.o chdijkstra.o

all: $(BIN)

rlparser.cpp: rlparser.rl
	ragel -G2 -o $@ $<

marble: $(BIN)
	g++ marble_debug.cpp -c -o marble_debug.o -Wall -g -std=c++0x -lglut -lGL -pthread -O3 -lmarblewidget -lQtGui 
	g++ marble_vis.cpp -c -o marble_vis.o -Wall -g -std=c++0x -lglut -lGL -pthread -O3 -lmarblewidget -lQtGui  
	g++  graphalgs.o parser.o graph.o ch.o rlparser.o openGLrender.o vis.o marble_vis.o marble_debug.o -o marble_debug -Wall -g -std=c++0x -lglut -lGL -pthread -O3 -lmarblewidget -lQtGui -Il /usr/lib/libGLEW.so /usr/lib/libSOIL.a /usr/lib/libQtCore.so.4

marble_noOpt: $(BIN)
	g++ marble_debug.cpp -c -o marble_debug.o -Wall -g -std=c++0x -lglut -lGL -pthread -lmarblewidget -lQtGui 
	g++ marble_vis.cpp -c -o marble_vis.o -Wall -g -std=c++0x -lglut -lGL -pthread -lmarblewidget -lQtGui  
	g++  graphalgs.o parser.o graph.o ch.o rlparser.o openGLrender.o vis.o marble_vis.o marble_debug.o -o marble_debug -Wall -g -std=c++0x -lglut -lGL -pthread -lmarblewidget -lQtGui -Il /usr/lib/libGLEW.so /usr/lib/libSOIL.a /usr/lib/libQtCore.so.4

sparser: sparser.cpp sparser.h
	g++ -o sparser -O3 sparser.cpp

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

# es werden all .cpp dateien kompiliert
$(OBJ): $(wildcard *.h) $(wildcard *.hpp)

clean:
	rm -f $(BIN) $(OBJ) marble_debug.o marble_vis.o marble_debug

#fucking...
.PHONY: all clean
