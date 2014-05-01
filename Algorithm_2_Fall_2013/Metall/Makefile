CC = g++
CFLAGS = -std=c++0x
RM = rm -rf
LIBRARY = /home/draif/Documents/Algorithm/Libraries
INCLUDE = -I$(LIBRARY)
OUT = Metall 

all: $(OUT)

$(OUT): 
	$(CC) metall.cpp -o $@ $(CFLAGS) $(INCLUDE)

clean:
	$(RM) $(OUT)
	$(RM) *.o
