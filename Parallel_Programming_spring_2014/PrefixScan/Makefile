CC = g++
CFLAGS = -std=c++11 -g 
BOOSTLIBRARY = /home/draif/boost/boost_1_55_0
INCLUDE = -I$(BOOSTLIBRARY) -I$(FROM)
FROM = /home/draif/Documents/Algorithm/lib/
THREADLIB = -pthread
BOOSTTHREAD = -lboost_system -lboost_thread 
OUT = PrefixScan 

all: $(OUT) clean

$(OUT): test.cpp PrefixScan.hpp
	$(CC) $(CFLAGS) $< -o $@  $(INCLUDE) $(BOOSTTHREAD) $(THREADLIB)

clean:
	rm -f *.gch   
