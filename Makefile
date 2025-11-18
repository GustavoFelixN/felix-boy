CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS =

SRC_DIR = src
BUILD_DIR = build
BUILD_SRC_DIR = $(BUILD_DIR)/src
TEST_BUILD_DIR = $(BUILD_DIR)/tests

TARGET = $(BUILD_DIR)/gb
TEST_TARGET = $(TEST_BUILD_DIR)/test_runner

#
# Source files (suporta subpastas)
SRC = $(shell find $(SRC_DIR) -name '*.cpp')
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_SRC_DIR)/%.o)

#
# Test sources
TEST_SRC = tests/test_registers.cpp \
           tests/test_cpu.cpp \
           tests/catch_amalgamated.cpp

TEST_OBJ = $(TEST_SRC:tests/%.cpp=$(TEST_BUILD_DIR)/%.o)

.PHONY: all clean test dirs
all: $(TARGET)

dirs:
	mkdir -p $(BUILD_SRC_DIR)
	mkdir -p $(TEST_BUILD_DIR)

#
# Build main target
$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) -o $@

#
# Compile source .cpp → build/src/**/.o (cria diretório automaticamente)
$(BUILD_SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

#
# Tests
test: $(OBJ) $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) $(BUILD_SRC_DIR)/registers.o $(BUILD_SRC_DIR)/cpu.o
	$(CXX) $(LDFLAGS) $^ -o $@

#
# Compile tests
$(TEST_BUILD_DIR)/%.o: tests/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -Itests -c $< -o $@

clean:
	rm -rf build
