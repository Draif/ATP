CC = g++
CFLAGS = -std=c++0x
RM = rm -rf
OUT = tests
BOOST = /home/andrey/Downloads/boost_1_54_0
INCLUDE = -I$(BOOST)

all: $(OUT)

$(OUT): tests.cpp stringAutomate.h
	$(CC) $< -o $@ $(CFLAGS) $(INCLUDE)

clean:
	$(RM) $(OUT)
