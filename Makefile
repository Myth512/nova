SRC_DIR = src
INC_DIR = include
OBJ_DIR = build
BIN_DIR = bin

TARGET = $(BIN_DIR)/nova

CC=gcc
OPT=-O0
DEPFLAGS=-MP -MD

CFLAGS= -std=c99 -Wall -Wextra -g -I $(INC_DIR) $(OPT) $(DEPFLAGS)

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)


# Build final executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -lm -o $(TARGET)

# Compile .c files into .o files and generate .d files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create direcries if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Include dependencies
-include $(DEPS)

.PHONY: all 
all: 
	$(TARGET)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
