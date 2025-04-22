CXX := g++
CXXFLAGS := -std=c++11 -Iinclude
LDFLAGS := -Llib -lcurses
BUILD_DIR := build
SRC_DIRS := src
SRCS := main.cpp $(wildcard $(SRC_DIRS)/*/*.cpp)
OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))


# Convert src/foo/bar.cpp -> build/src/foo/bar.o
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean

all: game

game: $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o out

debug: main.cpp $(wildcard src/*/*.cpp)
	g++ -std=c++11 main.cpp src/*/*.cpp -Iinclude -Llib -lcurses -g -o debug

copy: clean # create copy of project to be turned into tarball
	cp -R . ./copy
	rm -rf ./copy/.gitignore ./copy/.git ./copy/.DS_Store ./copy/compile_flags.txt
	mv ./copy ./young_alex.assignment-1.0

clean:
	rm -rf $(BUILD_DIR) out debug *.tar.gz log.txt ./young_alex.assignment* ./debug.dSYM
