CC = gcc
SRC = src/mm.c

all: mm_O0 mm_O2 mm_O3

mm_O0: $(SRC)
	$(CC) -O0 -o mm_O0 $(SRC)

mm_O2: $(SRC)
	$(CC) -O2 -o mm_O2 $(SRC)

mm_O3: $(SRC)
	$(CC) -O3 -march=native -o mm_O3 $(SRC)

clean:
	rm -f mm mm_O0 mm_O2 mm_O3