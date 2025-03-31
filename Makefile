CC=gcc
CFLAGS=-Wall -Wextra -I./include
LDFLAGS=-llua5.4 -lm

# Directories
SRC_DIR = src
INC_DIC = include
OBJ_DIR = obj
BIN_DIR = bin

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

# Clean object and binary files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets (not files)
.PHONY: all clean
