CC = g++
CFLAGS = -std=c++11 -g  `pkg-config --cflags opencv`  
INCLUDE = -I$(LIBRARY) `pkg-config --libs opencv` 
LIBRARY = /home/draif/Documents/ATP/Libraries
THREADLIB = -pthread
BOOSTTHREAD = -lboost_system -lboost_thread 
OUT = ImageSearch  

all: $(OUT) clean

$(OUT): main.cpp KDTree.hpp ImageTuple.hpp 
	$(CC) $(CFLAGS) $< ImageProcessing.cpp -o $@  $(INCLUDE) $(BOOSTTHREAD) $(THREADLIB)

test: testTree.cpp KDTree.hpp 
	$(CC) $(CFLAGS) -g -Wall $< -o test  $(INCLUDE) $(BOOSTTHREAD) $(THREADLIB)
clean:
	rm -f *.gch   
