CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = fs_sim
SRC = main.c fs_indexed.c

all: $(TARGET)

$(TARGET): $(SRC) fs_indexed.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean