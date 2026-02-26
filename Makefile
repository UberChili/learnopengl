CC = clang++
SRC_DIR = src
BIN_DIR = build
INCLUDE_DIR = include
SRC = $(wildcard $(SRC_DIR)/*.cpp)
SRC_NO_MAIN = $(filter-out $(SRC_DIR)/main.cpp, $(SRC))
OUT = $(BIN_DIR)/learnopengl
CFLAGS = -Wall -Werror -Wextra -std=c++23 -I$(INCLUDE_DIR)
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

# Test settings
TEST_SRC = $(wildcard tests/*.cpp)
TEST_OUT = $(BIN_DIR)/run_tests
TEST_LDFLAGS = -lgtest -lgtest_main -pthread

# Platform-specific settings
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CFLAGS +=
    LDFLAGS +=
else ifeq ($(UNAME_S),Linux)
    CFLAGS +=
    LDFLAGS +=
else
    CFLAGS +=
    LDFLAGS +=
endif

# Default target
all: $(OUT)

# Build target
# $(OUT): $(SRC) | $(BIN_DIR)
# 	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)
# New: 
$(OUT): $(SRC) $(BIN_DIR)/glad.o | $(BIN_DIR)
	$(CC) $(SRC) $(BIN_DIR)/glad.o -o $(OUT) $(CFLAGS) $(LDFLAGS)

# Create build directory
$(BIN_DIR):
	mkdir -p $(BIN_DIR)
$(BIN_DIR)/glad.o: $(SRC_DIR)/glad.c | $(BIN_DIR)
	clang -c $(SRC_DIR)/glad.c -o $(BIN_DIR)/glad.o -I$(INCLUDE_DIR)

# Clean build artifacts
clean:
	rm -rf $(BIN_DIR)

# Run
run: $(OUT)
	./$(OUT) $(ARGS)

run-args: $(OUT)
	./$(OUT) $(ARGS)

# Build and run tests
$(TEST_OUT): $(TEST_SRC) $(SRC_NO_MAIN) | $(BIN_DIR)
	$(CC) $(TEST_SRC) $(SRC_NO_MAIN) -o $(TEST_OUT) $(CFLAGS) $(TEST_LDFLAGS)

test: $(TEST_OUT)
	./$(TEST_OUT)

.PHONY: all clean run run-args test
