CC = gcc
CFLAGS = -Wall -Wextra -pthread

# Directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
EXAMPLE_DIR = example

# Source files
SRC_FILES = $(SRC_DIR)/memory_allocation.c
TEST_FILES = $(TEST_DIR)/tests.c
EXAMPLE_FILES = $(EXAMPLE_DIR)/example.c

# Targets
all: $(BUILD_DIR)/example $(BUILD_DIR)/tests

example: $(BUILD_DIR)/example
	$(BUILD_DIR)/tests

tests: $(BUILD_DIR)/tests
	$(BUILD_DIR)/tests

# Build example
$(BUILD_DIR)/example: $(SRC_FILES) $(EXAMPLE_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -o $@ $^

# Build tests
$(BUILD_DIR)/tests: $(SRC_FILES) $(TEST_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -o $@ $^ -lcriterion

clean:
	rm -rf $(BUILD_DIR)
