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
TEST_EXEC_INTEGRATION = $(BIN_DIR)/test_integration_cli
TEST_EXEC_UTIL = $(BIN_DIR)/test_unit_util

test_integration_cli: $(EXEC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(TEST_DIR)/integration_cli.c -o $(TEST_EXEC_INTEGRATION) $(shell pkg-config --cflags --libs check)
	$(TEST_EXEC_INTEGRATION)
test_unit_util: tests/unit_util.c src/c/util.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(TEST_DIR)/unit_util.c src/c/util.c -o $(TEST_EXEC_UTIL) $(shell pkg-config --cflags --libs check)
	$(TEST_EXEC_UTIL)

# Clean object and binary files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) 

# Phony targets (not files)
.PHONY: all clean test test_unit_util test_integration_cli
