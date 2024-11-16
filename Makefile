BIN_DIR=./bin
PROGRAM=$(BIN_DIR)/sudoku

CC=g++
CFLAGS=-c -O3 -fopenmp
LDFLAGS=-fopenmp

OBJS=$(BIN_DIR)/SudokuBoard.o $(BIN_DIR)/sudoku.o

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/%.o: %.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ 

$(BIN_DIR): 
	mkdir -p $@

clean:
ifeq ($(OS),Windows_NT)
	del /Q $(subst /,\,$(BIN_DIR))\*.o
	del /Q $(subst /,\,$(PROGRAM)).exe
else
	rm -rf $(PROGRAM) *.o
endif

wipe: clean
ifeq ($(OS),Windows_NT)
	del /Q *.out *.err
else
	rm -rf *.out *.err
endif	
