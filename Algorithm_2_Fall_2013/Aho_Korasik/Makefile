CC = g++
CFLAGS = -std=c++0x
OUT = Aho_Corasick 

all: $(OUT)

$(OUT): 
	$(CC) forbiddenWords.cpp -o $@ $(CFLAGS)
clean:
	rm -f $(OUT) forbiddenWords.cpp.gch
