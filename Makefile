SRC = main.cpp function.cpp

BUILD_RELEASE = build/release
BUILD_DEBUG = buid/debug

OBJ_RELEASE = $(patsubst %.cpp, $(BUILD_RELEASE)/%.o, $(SRC))
OBJ_DEBUG = $(patsubst %.cpp, $(BUILD_DEBUG)/%.o, $(SRC))

CXX_RELEASE_FLAG = -c -O3 -march=native -ffast-math -funroll-loops -flto -DNDEBUG
CXX_DEBUG_FLAG = -c -pthread -g3 -O0 -Wall -Wextra -fno-omit-frame-pointer -fno-inline


release: $(OBJ_RELEASE)
	g++ -o a.out -pthread $^

debug: $(OBJ_DEBUG)
	g++ -o b.out -pthread $^

$(BUILD_DEBUG)/%.o: %.cpp
	mkdir -p $(BUILD_DEBUG)
	g++ $(CXX_DEBUG_FLAG) $< -o $@

$(BUILD_RELEASE)/%.o: %.cpp
	mkdir -p $(BUILD_RELEASE)
	g++ $(CXX_RELEASE_FLAG) $< -o $@




