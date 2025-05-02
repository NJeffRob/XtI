CC=gcc
CFLAGS=-Wall -Wextra -I./include
LDFLAGS=-llua5.4 -lm

# Directories
SRC_DIR = src/c
INC_DIC = include
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

# Source and object files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Output binary
EXEC = $(BIN_DIR)/xti

# Default target: 
all: $(EXEC)

$(EXEC): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)  
	$(CC) $(CFLAGS) -c $< -o $@

# Tests 
TEST_EXEC_CLI = $(BIN_DIR)/test_cli
TEST_EXEC_ARG_LENGTH = $(BIN_DIR)/test_arg_length

test_cli: $(EXEC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(TEST_DIR)/cli.c -o $(TEST_EXEC_CLI) $(shell pkg-config --cflags --libs check)
	$(TEST_EXEC_CLI)
test_arg_length: tests/arg_length.c src/c/util.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(TEST_DIR)/arg_length.c src/c/util.c -o $(TEST_EXEC_ARG_LENGTH) $(shell pkg-config --cflags --libs check)
	$(TEST_EXEC_ARG_LENGTH)

# Run all tests
test: test_arg_length test_cli

# Clean object and binary files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) 

# Phony targets (not files)
.PHONY: all clean test test_arg_length test_cli
