# Makefile for robot-logger project

# Compiler
CXX = g++
CXXFLAGS = -Iinclude -std=c++17 -Wall -Wextra -O2

# Source files
SRCS = src/robot-logger.cpp src/User.cpp
HEADERS = include/User.h include/menuUtils.h

# Output executable
TARGET = robot-logger

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link objects into executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile each .cpp into .o
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean