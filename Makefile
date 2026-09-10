CC = gcc
CFLAGS = -I . -Wall -Wextra
LDFLAGS = -ljansson

# Directories
BUILD_DIR = build

# Object files
OBJECTS = $(BUILD_DIR)/node.o \
          $(BUILD_DIR)/main.o \
          $(BUILD_DIR)/dfs.o \
          $(BUILD_DIR)/bfs.o \
		  $(BUILD_DIR)/greedy.o \
          $(BUILD_DIR)/path_util.o \
          $(BUILD_DIR)/string_util.o

# Output executable
TARGET = searchAlgs

# Default target
all: $(BUILD_DIR) $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files to object files
$(BUILD_DIR)/node.o: src/node.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/dfs.o: src/algs/dfs.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/bfs.o: src/algs/bfs.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/greedy.o: src/algs/greedy.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/path_util.o: src/algs/path_util.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/string_util.o: str_util/string_util.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) 

# Phony targets
.PHONY: all clean
