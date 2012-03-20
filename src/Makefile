CC = g++
#CXXFLAGS = -Wall -g -O3 -std=c++0x #-lmarblewidget -lQtGui
CFLAGS = -Wall -g -O3
LDFLAGS = #-Il /usr/lib/libQtCore.so.4
BIN =pferd

OBJDIR=obj

# zu jeder X.cpp datei, die kompiliert wird, hier in
# OBJ eine entsprechende X.o file anlegen lassen
CPP =pferd.cpp graphalgs.cpp parser.cpp graph.cpp ch.cpp rlparser.c vis.cpp
OBJ =pferd.o graphalgs.o parser.o graph.o ch.o rlparser.o vis.o

OGLBIN =main_vis_debug
OGLOBJ =main_vis_debug.o openGLrender.o parser.o
OGLFLAGS = -Wall -g -O3 -std=c++0x -lglut -lGL 

CXXFLAGS = $(OGLFLAGS)

opengl : $(OGLBIN)

$(OGLBIN) : $(OGLOBJ)
	$(CC) $(OGLFLAGS) -o $(OGLBIN) $(OGLOBJ) /usr/lib/libGLEW.so /usr/lib/libSOIL.a

$(OGLOBJ) : $(wildcard *.h) $(wildcard *.hpp)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CXXFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

# es werden all .cpp dateien kompiliert
$(OBJ): $(wildcard *.h) $(wildcard *.hpp)

clean:
	rm -f $(BIN) $(OBJ) *.o

#fucking...
.PHONY: all clean
