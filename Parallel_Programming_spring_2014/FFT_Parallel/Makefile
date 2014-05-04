CC = g++
CFLAGS = -std=c++11    
INCLUDE = -I$(LIBRARY) 
LIBRARY = /home/draif/Documents/ATP/Libraries
THREADLIB = -pthread
BOOSTTHREAD = -lboost_system -lboost_thread 
OUT = FFT  

all: $(OUT) clean

$(OUT): test.cpp FFT_Parallel.hpp  Polynomial.hpp
	$(CC) $(CFLAGS) $< -o $@  $(INCLUDE) $(BOOSTTHREAD) $(THREADLIB)

clean:
	rm -f *.gch   
