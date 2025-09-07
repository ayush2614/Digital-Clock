# Makefile for Digital Clock Application

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

TARGET = digital_clock
SOURCES = src/digital_clock.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Digital Clock built successfully! Run with: ./$(TARGET)"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Cleaned build artifacts."

run: $(TARGET)
	./$(TARGET)
