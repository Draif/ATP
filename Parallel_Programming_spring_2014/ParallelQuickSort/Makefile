CC = g++
CFLAGS = -std=c++11 
BOOSTLIBRARY = /home/draif/boost/boost_1_55_0
INCLUDE = -I$(BOOSTLIBRARY) -I$(FROM)
FROM = /home/draif/Documents/Algorithm/lib/
THREADLIB = -pthread
BOOSTTHREAD = -lboost_system -lboost_thread 
OUT = QuickSort 

all: $(OUT)

$(OUT):
	$(CC) $(CFLAGS) tests.cpp ParallelQuickSort.hpp -o $@  $(INCLUDE) $(BOOSTTHREAD) $(THREADLIB)  
