CXX := mpic++
CXXFLAGS := -Wall -O3 -Wextra -std=c++11 -g #-I/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi -I/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi/opal/mca/event/libevent2022/libevent -I/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi/opal/mca/event/libevent2022/libevent/include -I/usr/lib/x86_64-linux-gnu/openmpi/include -pthread -L/usr//lib -L/usr/lib/x86_64-linux-gnu/openmpi/lib -lmpi_cxx -lmpi
# DIRECTORIES
BASE_DIR := .
SRC_DIR := $(BASE_DIR)/src
HEADER_DIR := $(BASE_DIR)/include
LIB_DIR := $(BASE_DIR)/lib
BUILD_DIR := $(BASE_DIR)/build
BIN_DIR := $(BASE_DIR)/bin

# FILES
TARGET := main
LIBC_FILES := $(wildcard $(LIB_DIR)/*.c)
LIBH_FILES := $(wildcard $(LIB_DIR)/*.h)
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(BASE_DIR)/main.cpp
LIB_OBJ_FILES := $(patsubst $(LIB_DIR)/%.c, $(BUILD_DIR)/%.o, $(LIBC_FILES))
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES)) $(LIB_OBJ_FILES)

BIN := $(BIN_DIR)/$(TARGET)

$(BIN): $(OBJ_FILES) $(wildcard $(BUILD_DIR)/*.o)
	@echo "Linking..."
	@echo $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp main.cpp $(HEADER_DIR)/%.hpp 
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -pg -c -o $@ $<

# generate library object files manually
build/ran2.o: lib/ran2.c lib/ran2.h
	$(CXX) $(CXXFLAGS) -pg -c -o $@ $<
build/pool.o: lib/pool.c lib/pool.h
	$(CXX) $(CXXFLAGS) -pg -c -o $@ $<
build/squirrel-functions.o: lib/squirrel-functions.c lib/squirrel-functions.h
	$(CXX) $(CXXFLAGS) -pg -c -o $@ $<
	
#lib: $(LIBC_FILES) $(LIBH_FILES)
#	@echo "Building library"
#	$(CXX) $(CXXFLAGS) -pg -c -o 

all: $(BIN) $(BUILD_DIR)%.o #lib
	@echo "Finished Compiling"

test: 
	mpic++ test/test.c lib/pool.c lib/ran2.c lib/squirrel-functions.c -o bin/test

clean:
	rm -rf ./build/*.o

.PHONY: clean test
