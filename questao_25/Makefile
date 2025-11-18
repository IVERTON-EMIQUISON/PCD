# Compilador
CC = gcc
CFLAGS = -Wall -g -fopenmp

TARGET = omp_trap3
SRC = omp_trap3.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
