CXX := g++
CXXLFAGS := -Wall -Wextra -g -I
# DIRECTORIES
BASE_DIR := .
SRC_DIR := $(BASE_DIR)/src
HEADER_DIR := $(BASE_DIR)/include
LIBS := $(BASE_DIR)/lib
BUILD_DIR := $(BASE_DIR)/build
BIN_DIR := $(BASE_DIR)/bin

# FILES
TARGET := main
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(BASE_DIR)/main.cpp
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

BIN := $(BIN_DIR)/$(TARGET)

$(BIN): $(OBJ_FILES) $(wildcard $(BUILD_DIR)/*.o)
	@echo "Linking..."
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp main.cpp $(HEADER_DIR)/%.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -pg -c -o $@ $<

all: $(BIN) $(BUILD_DIR)%.o
	@echo "Finished Compiling"

clean:
	rm -rf ./build/*.o

.PHONY: clean
