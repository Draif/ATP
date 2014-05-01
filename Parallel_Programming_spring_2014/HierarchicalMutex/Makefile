CC = clang++
CFLAGS = -std=c++11 
BOOSTLIBRARY = /home/draif/boost/boost_1_55_0
INCLUDE = -I$(BOOSTLIBRARY) -I$(FROM)
FROM = /home/draif/Documents/Algorithm/lib/
THREADLIB = -pthread
BOOSTTHREAD = -lboost_system -lboost_thread 
OUT =  HM

all: $(OUT) clean

$(OUT):
	$(CC) $(CFLAGS) tests.cpp HierarchicalMutex.hpp  $(INCLUDE) $(BOOSTTHREAD) $(THREADLIB)

clean:
	rm *.gch
	

