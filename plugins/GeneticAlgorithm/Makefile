# ==== USER SETTINGS ====
SRC  = geneticAlgorithm.cpp
OUT  = genetic.so
ROUTE_INC = ../../include         # <--- VERY IMPORTANT
# ========================

CXX = g++
CXXFLAGS = -fPIC -O2 -Wall -fvisibility=default

INCLUDES = \
    -I$(ROUTE_INC) \
    -I/usr/include/python3.10 \
    -I$(HOME)/.local/lib/python3.10/site-packages/numpy/_core/include

LIBS = -lyaml-cpp -lpython3.10

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -shared $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)

.PHONY: all clean
