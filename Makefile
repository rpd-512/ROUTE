# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -pthread

# Source and output
SRC = main.cpp
OUT = ROUTE_linux

# Python/numpy includes and libs
PY_INCLUDES := $(shell python3-config --includes)
PY_LIBS := $(shell python3-config --ldflags) -lpython3.10
NUMPY_INCLUDE := -I$(shell python3 -c "import numpy; print(numpy.get_include())")

# Eigen and other global includes
GLOBAL_INCLUDES = -I/usr/include/eigen3 -I/usr/local/include

# Build target
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) \
		$(PY_INCLUDES) $(NUMPY_INCLUDE) $(GLOBAL_INCLUDES) \
		-o $(OUT) \
		$(PY_LIBS)

# Debug target
gdb: $(OUT)
	gdb ./$(OUT)

# Clean target
clean:
	rm -f $(OUT)
