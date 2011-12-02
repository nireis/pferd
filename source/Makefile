CC = g++
CFLAGS = -Wall -g
LDFLAGS = 
BIN =pferd

OBJ =pferd.o

all: $(BIN) $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

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
