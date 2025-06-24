# Makefile for calc program

# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -O2

# Source files
SRCS := calc.cpp logic.cpp
OBJS := $(SRCS:.cpp=.o)

# Output
TARGET := build/calc

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f *.o
	rm -rf build

.PHONY: all clean
