CC = g++
CFLAGS = -std=c++11 -Wall
LIBRARY = -lboost_system
INCLUDE = $(LIBRARY)
OUT = PriorityQueue 

all: $(OUT) clean

$(OUT): PriorityQueue.hpp tests.cpp BinaryHeap.hpp BinominalHeap.hpp PriorityQueueException.hpp
	$(CC) tests.cpp -o $@ $(CFLAGS) $(INCLUDE)

clean:
	rm -f  *.gch
