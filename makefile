# Compiler
CC = gcc

# Compiler flags
CFLAGS = -pthread -lm -Wall -Wextra -O2

# Executable name
TARGET = thr_atomic

# Source files
SRC = thr_atomic.c

# Default target (build)
all: $(TARGET)

# Build the program
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -lm


# Clean up build files
clean:
	rm -f $(TARGET)