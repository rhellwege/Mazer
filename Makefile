EXE=mazer
_DEPS = Cell.h common.h ImageWriter.h Maze.h
_OBJ = Cell.o ImageWriter.o main.o Maze.o

IDIR =include/
SRC=src/
ODIR=obj

CC=g++
CFLAGS=-I$(IDIR) -lGL -g -Wall -Wformat `pkg-config --static --libs glfw3`

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRC)%.cpp $(DEPS)
	@mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)
 
$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o 
