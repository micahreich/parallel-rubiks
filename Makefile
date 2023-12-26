# Variables
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -pthread
SRC_DIR = src
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin

# Targets
all: $(BIN_DIR)/main

test: $(BIN_DIR)/test_rubiks_cube

$(BIN_DIR)/main: $(OBJ_DIR)/main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR)/test_rubiks_cube: $(TEST_DIR)/test_rubiks_cube.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/main $(BIN_DIR)/test_rubiks_cube