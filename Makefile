# Variables
CC = g++
CFLAGS = -Wall -Wextra -g
TARGET = nsh

# Targets
all: $(TARGET)

$(TARGET): nsh.cpp
	$(CC) $(CFLAGS) nsh.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)

