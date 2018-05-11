CC = gcc
CFLAGS = -Wall -O2
LDLIBS = -lglpk -lm

sudoku:	sudoku.o
	$(CC) -o sudoku_v2 sudoku_v2.o $(LDLIBS)
sudoku.o:	sudoku_v2.c
	$(CC) -c sudoku_v2.c $(CFLAGS)

clean:
	rm -f  *.o *~
