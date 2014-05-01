CC = g++
CFLAGS = -std=c++11 -g
LIBRARY = /home/draif/boost/boost_1_55_0
INCLUDE = -I$(LIBRARY) -I$(FROM)
FROM = /home/draif/Documents/Algorithm/lib/
THREADLIB = -pthread
BOOSTTHREAD = -lboost_system -lboost_thread 
OUT = ThLog

all: $(OUT)

$(OUT):
	$(CC) $(CFLAGS) tests.cpp -o $@ $(INCLUDE) $(BOOSTTHREAD) $(THREADLIB)  
