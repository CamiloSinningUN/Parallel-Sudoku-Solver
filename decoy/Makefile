# Compiler and flags
CXX = g++
CXXFLAGS = -fopenmp -Wall -Wextra -O2

# Executable name
TARGET = openmp_tasks

# Source file
SRC = simple.cpp

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean up
clean:
	rm -f $(TARGET)
