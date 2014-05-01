CC = g++
CFLAGS = -std=c++11   
INCLUDE = -I$(LIBRARY) -I$(MATRIX)
MATRIX = /home/draif/Documents/ATP/Algorithm_1_2012_2013/TMatrix
LIBRARY = /home/draif/Documents/ATP/Libraries
THREADLIB = -pthread
BOOSTTHREAD = -lboost_system -lboost_thread 
OUT = LUP  

all: $(OUT) clean

$(OUT): test.cpp LUP_decomposition_parallel.hpp
	$(CC) $(CFLAGS) $< -o $@  $(INCLUDE) $(BOOSTTHREAD) $(THREADLIB)

clean:
	rm -f *.gch   
