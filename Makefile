# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2
TARGET = main
SRC = main.c

.PHONY: all clean

all: clean $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -E -o $(TARGET).i $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
