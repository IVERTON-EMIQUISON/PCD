CC = gcc
CFLAGS = -g -Wall -fopenmp

PROGS = test_atomic omp_26

all: $(PROGS)

test_atomic: test_atomic.c
	$(CC) $(CFLAGS) -o test_atomic test_atomic.c

omp_26: omp_26.c
	$(CC) $(CFLAGS) -o omp_26 omp_26.c

run: all
	sh run_all.sh

clean:
	rm -f $(PROGS)
