# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude -std=c++17

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
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
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
