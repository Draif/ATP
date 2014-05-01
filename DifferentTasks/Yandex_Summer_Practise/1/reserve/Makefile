BISON=bison
FLEX=flex
CXX=g++
CFLAGS = -std=c++03

all: text_parser 

grammatics.tab.c grammatics.tab.h: grammatics.y
	$(BISON) -d $<

lex.yy.c: reserve.l
	$(FLEX) $<

text_parser: lex.yy.c grammatics.tab.c grammatics.tab.h main.cpp libstemmer.o
	$(CXX) $^ $(CFLAGS)

clean:
	rm -f lex.yy.c grammatics.tab.c grammatics.tab.h 
