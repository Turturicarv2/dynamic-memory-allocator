# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread -g
LDFLAGS = -pthread

# Source files and output
SRC = memory_allocation.c
LIB = libmemory.a

# Default target
all: $(LIB)

# Build static library
$(LIB): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	ar rcs $(LIB) memory_allocation.o

# Tests target
tests: $(SRC) tests.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o tests $(SRC) tests.c -lcriterion
	./tests

# Clean up
clean:
	rm -f *.o $(LIB) tests
