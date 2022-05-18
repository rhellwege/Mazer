IDIR =include/
CC=g++
CFLAGS=-I$(IDIR)
SRC=src/

ODIR=obj

_DEPS = Cell.h common.h ImageWriter.h Maze.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Cell.o ImageWriter.o main.o Maze.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SRC)%.cpp $(DEPS)
	@mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

mazer: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o 