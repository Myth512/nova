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

test:
	python3 tests/run_tests.py

.PHONY: gperf
gperf:
	gperf gperf/keywords.txt > include/keywords.h
	gperf gperf/methods_string.txt > include/methods_string.h
	gperf gperf/methods_int.txt > include/methods_int.h
	gperf gperf/methods_bool.txt > include/methods_bool.h
	gperf gperf/methods_none.txt > include/methods_none.h
	gperf gperf/methods_float.txt > include/methods_float.h
	gperf gperf/methods_list.txt > include/methods_list.h
	gperf gperf/methods_tuple.txt > include/methods_tuple.h