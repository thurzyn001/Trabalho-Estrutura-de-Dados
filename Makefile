# Makefile — Merge Sort Recursivo em C (Estrutura de Dados - 4ª Fase)

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -O2 -Iinclude
TARGET  = mergesort
SRCS    = src/main.c src/registro.c src/mergesort.c src/metricas.c src/leitor_csv.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

auto: $(TARGET)
	./$(TARGET) --auto

clean:
	rm -f $(TARGET) $(TARGET).exe *.o
