CXX = g++
CXXFLAGS = -Wall -std=c++17 -Itests
LDFLAGS =

BUILD_DIR = build
TEST_BUILD_DIR = build/tests

TARGET = $(BUILD_DIR)/gb
TEST_TARGET = $(TEST_BUILD_DIR)/test_runner

SRC = main.cpp cpu.cpp registers.cpp
OBJ = $(SRC:%=$(BUILD_DIR)/%.o)

TEST_SRC = tests/test_registers.cpp tests/catch_amalgamated.cpp registers.cpp
TEST_OBJ = $(TEST_SRC:%=$(BUILD_DIR)/%.o)

.PHONY: all clean test dirs
all: dirs $(TARGET)
dirs:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(TEST_BUILD_DIR)

#
# Build
$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Tests
test: dirs $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ)
	$(CXX) $(LDFLAGS) $(TEST_OBJ) -o $@

$(TEST_BUILD_DIR)/%.cpp.o: %.cpp
	$(CXX) $(CXXFLAGS) -Itests -c $< -o $@


clean:
	rm -rf build
