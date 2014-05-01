CC = g++
CFLAGS = -std=c++0x
LIBRARY = /home/draif/boost/boost_1_55_0
INCLUDE = -I$(LIBRARY)
OUT = HeapLibrary 

all: $(OUT)

$(OUT): 
	$(CC) HeapLibrary.h tests.cpp -o $@ $(CFLAGS) $(INCLUDE)
clean:
	rm -f $(OUT) HeapLibrary.h.gch
