BISON=bison
FLEX=flex
CXX=g++
CFLAGS = -std=c++0x

all: text_parser 

grammatics.tab.c grammatics.tab.h: grammatics.y
	$(BISON) -d $<

lex.yy.c: reserve.l
	$(FLEX) $<

text_parser: lex.yy.c grammatics.tab.c grammatics.tab.h indexer.h SearcherLibrary.h main.cpp Stem.h libstemmer.h libstemmer.o
	$(CXX) $^ -o $@ 

clean:
	rm -f lex.yy.c grammatics.tab.c grammatics.tab.h 
