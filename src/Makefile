CC = gcc
CFLAGS = -Wall -O2 -g
LDFLAGS = -lm

SRC = src/main.c src/graph.c
TARGET = solver

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all clean
