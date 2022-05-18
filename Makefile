IDIR =include/
CC=g++
CFLAGS=-I$(IDIR)
SRC=src/

ODIR=obj

_DEPS = Cell.h common.h Image.h Maze.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Cell.o Image.o main.o Maze.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SRC)%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mazer: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o 