# ==== USER SETTINGS ====
SRC  = geneticAlgorithm.cpp   # <-- your .cpp file
OUT  = genetic.so             # <-- your .so output file
# ========================

CXX = g++
CXXFLAGS = -fPIC -O2 -Wall

PY_INC = -I/usr/include/python3.10 \
         -I/usr/include/python3.10 \
         -I$(HOME)/.local/lib/python3.10/site-packages/numpy/_core/include

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(PY_INC) -shared $< -o $@

clean:
	rm -f $(OUT)

.PHONY: all clean

