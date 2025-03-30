# Variables
CC = g++
CFLAGS = -Wall -Wextra -g
TARGET = fsh.out

# Targets
all: $(TARGET)

$(TARGET): main.cpp
	$(CC) $(CFLAGS) main.cpp -o $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
