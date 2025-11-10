# Makefile for robotLogger project (cross-platform)

# Compiler
CXX = g++
CXXFLAGS_BASE = -Iinclude -std=c++17 -Wall -Wextra -O2

# Detect OS
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)  # macOS
    OPENCV_INC = $(shell pkg-config --cflags opencv4)
    OPENCV_LIB = -L/opt/homebrew/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs
    ZBAR_INC = $(shell pkg-config --cflags zbar)
    ZBAR_LIB = -L/opt/homebrew/lib -lzbar
else                       # Linux
    OPENCV_INC = `pkg-config --cflags opencv4`
    OPENCV_LIB = `pkg-config --libs opencv4`
    ZBAR_INC = `pkg-config --cflags zbar`
    ZBAR_LIB = `pkg-config --libs zbar`
endif

CXXFLAGS = $(CXXFLAGS_BASE) $(OPENCV_INC) $(ZBAR_INC)
LIBS = $(OPENCV_LIB) $(ZBAR_LIB) -lsqlite3

# Source files
SRCS = src/robotLogger.cpp \
       src/menuUtils.cpp \
       src/User.cpp \
       src/robotUtils.cpp \
       src/userUtils.cpp \
       src/dbUtils.cpp \
       src/checkOut.cpp \
       src/checkIn.cpp \
       src/barcodeUtils/barcodeScanner.cpp

# Headers (for dependency tracking)
HEADERS = include/User.h \
          include/menuUtils.h \
          include/robotUtils.h \
          include/userUtils.h \
          include/dbUtils.h \
          include/checkOut.h \
          include/checkIn.h \
          src/barcodeUtils/barcodeScanner.h

# Object files
OBJS = $(SRCS:.cpp=.o)

# Output executable
TARGET = robotLogger

# Default target
all: $(TARGET)

# Link objects into executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS)

# Compile each .cpp into .o
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean