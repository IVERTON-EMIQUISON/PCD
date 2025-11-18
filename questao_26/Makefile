CC = gcc
CFLAGS = -g -Wall -fopenmp
LFLAGS = -lm

EXECS = test_atomic omp_26

all: $(EXECS)

test_atomic: test_atomic.c
	$(CC) $(CFLAGS) -o test_atomic test_atomic.c $(LFLAGS)

omp_26: omp_26.c
	$(CC) $(CFLAGS) -o omp_26 omp_26.c $(LFLAGS)

clean:
	rm -f $(EXECS) *.o
