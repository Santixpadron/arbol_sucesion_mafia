CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
SRC_DIR = src
BIN_DIR = bin
TARGET = $(BIN_DIR)/arbol_sucesion.exe

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/arbol.cpp $(SRC_DIR)/csv_parser.cpp $(SRC_DIR)/sucesion.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	del /Q $(BIN_DIR)\arbol_sucesion.exe 2>nul

.PHONY: all clean
