# Compiler and flags
CXX = gcc
CXXFLAGS = -Wall -Wextra -Iinclude -std=c11

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
TARGET = $(BUILD_DIR)/terraria

# Libraries (e.g., SDL2)
LIBS = -lSDL2 -lSDL2_image -lm

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LIBS)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
