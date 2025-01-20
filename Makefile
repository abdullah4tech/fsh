# Variables
CC = g++
CFLAGS = -Wall -Wextra -g
TARGET = fsh

# Targets
all: $(TARGET)

$(TARGET): main.cpp
	$(CC) $(CFLAGS) main.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)

