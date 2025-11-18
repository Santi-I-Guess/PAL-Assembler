SRC_FILES = $(wildcard *.cpp)
BUILD_DIR = ../build
OBJ_FILES = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

CXX = g++
CXXFLAGS_DEBUG = -g
CXXFLAGS_WARN  = -Wall -Werror -Wextra -Wconversion -Wdouble-promotion \
				 -Wunreachable-code -Wshadow -Wpedantic
CPPVERSION = -std=c++17

all: $(OBJ_FILES)

# order only prerequisite
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CPPVERSION) $(CXXFLAGS_DEBUG) $(CXXFLAGS_WARN) -o $@ -c $<

.DEFAULT: all
