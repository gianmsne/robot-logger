# Makefile for robotLogger project

# Compiler
CXX = g++
CXXFLAGS = -Iinclude -std=c++17 -Wall -Wextra -O2

# Source files
SRCS = src/robotLogger.cpp src/menuUtils.cpp src/User.cpp src/robotUtils.cpp src/userUtils.cpp src/dbUtils.cpp src/checkOut.cpp src/checkIn.cpp

# Headers (for dependency tracking)
HEADERS = include/User.h include/menuUtils.h include/robotUtils.h include/userUtils.h include/dbUtils.h include/checkOut.h include/checkIn.h

# Output executable
TARGET = robotLogger

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link objects into executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) -lsqlite3

# Compile each .cpp into .o
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean