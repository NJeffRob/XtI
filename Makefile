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

# Test targets
test_cli: $(EXEC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(TEST_DIR)/cli.c -o $(BIN_DIR)/test_cli $(shell pkg-config --cflags --libs check)
	$(BIN_DIR)/test_cli

test_arg_length: tests/arg_length.c src/c/util.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(TEST_DIR)/arg_length.c src/c/util.c -o $(BIN_DIR)/test_arg_length $(shell pkg-config --cflags --libs check)
	$(BIN_DIR)/test_arg_length

test_file_handler: $(TEST_DIR)/file_handler.c src/c/file_handler.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(TEST_DIR)/file_handler.c src/c/file_handler.c -o $(BIN_DIR)/test_file_handler $(shell pkg-config --cflags --libs check)
	$(BIN_DIR)/test_file_handler

# Run all tests
test: test_arg_length test_cli test_file_handler

# Clean object and binary files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) 

# Phony targets (not files)
.PHONY: all clean test test_arg_length test_cli test_file_handler
