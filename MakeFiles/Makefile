CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -g
LDLIBS = -pthread

SRCS = nscan.cpp menus.cpp functionality.cpp
TARGET = nscan

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDLIBS)

clean:
	rm -f $(TARGET)
