CXX = g++
CXXFLAGS = -Wall -std=c++17 -Itests
LDFLAGS =

TARGET = gb
SOURCES = main.cpp cpu.cpp registers.cpp
OBJECTS = $(SOURCES:.cpp=.o)

TEST_TARGET = test_runner
TEST_SOURCES = tests/test.cpp tests/catch_amalgamated.cpp registers.cpp

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET):
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) -o $(TEST_TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) $(TEST_TARGET)
