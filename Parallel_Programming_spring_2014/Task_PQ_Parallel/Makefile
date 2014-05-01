CC = g++
CFLAGS = -std=c++11 
BOOSTLIBRARY = /home/draif/boost/boost_1_55_0
MyLib = /home/draif/Documents/Algorithm/lib
INCLUDE = -I$(BOOSTLIBRARY) -I$(MyLib)
THREADLIB = -pthread
BOOSTTHREAD = -lboost_system -lboost_thread
OUT = ThPool

all: $(OUT) clean

$(OUT):
	$(CC) $(CFLAGS) main.cpp -o $@ $(BOOSTTHREAD) $(INCLUDE) $(THREADLIB)

clean:
	rm *.gch -f 
