# Compilador e flags
CC = gcc
CFLAGS = -Wall -g -fopenmp

# Nome do executável
TARGET = omp_23

# Fonte
SRC = omp_23.c

# Regra padrão
all: $(TARGET)

# Compilação
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Execução padrão (4 threads só para teste)
run: $(TARGET)
	./$(TARGET) 4

# Limpar arquivos gerados
clean:
	rm -f $(TARGET)
