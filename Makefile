CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wpedantic -O2

SRCS := calc.cpp logic.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := build/calc

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJS): calc.hpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	python3 tests/test_calc.py

clean:
	rm -f $(OBJS)
	rm -rf build

.PHONY: all clean test
