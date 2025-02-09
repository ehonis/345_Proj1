
CC = gcc

CFLAGS = -pthread -Wall -Wextra -O2


TARGET_ATOMIC = thr_atomic
TARGET_REDUCE = thr_reduce

SRC_ATOMIC = thr_atomic.c
SRC_REDUCE = thr_reduce.c

all: $(TARGET_ATOMIC) $(TARGET_REDUCE)

$(TARGET_ATOMIC): $(SRC_ATOMIC)
	$(CC) $(CFLAGS) -o $(TARGET_ATOMIC) $(SRC_ATOMIC) -lm

$(TARGET_REDUCE): $(SRC_REDUCE)
	$(CC) $(CFLAGS) -o $(TARGET_REDUCE) $(SRC_REDUCE) -lm


# Clean up build files
clean:
	rm -f $(TARGET_ATOMIC) $(TARGET_REDUCE)