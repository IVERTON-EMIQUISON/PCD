# Compilador e flags
CC = gcc
CFLAGS = -Wall -g -fopenmp

# Nome do executável
TARGET = omp_trap_1

# Arquivos fonte
SRC = omp_trap_1.c

# Regra padrão
all: $(TARGET)

# Regra para compilar o código
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Executar com 4 threads (pode alterar)
run: $(TARGET)
	./$(TARGET) 4

# Limpar arquivos gerados
clean:
	rm -f $(TARGET)
