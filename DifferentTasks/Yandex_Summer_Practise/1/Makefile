BISON = bison
FLEX = flex
CXX = g++
CFLAGS = -std=c++03  
OUT = Searcher
BOOST = -lboost_system

all: Searcher 

Searcher: grammatics.tab.o lex.yy.o main.o libstemmer.o
	$(CXX) $^ -o $(OUT) $(CFLAGS)

lex.yy.c: lexer.l
	$(FLEX) $^

grammatics.tab.o: grammatics.tab.c grammatics.tab.h 
	$(CXX) -c grammatics.tab.c $(CFLAGS)    

grammatics.tab.c: grammatics.y
	$(BISON) -d $^

lex.yy.o: lex.yy.c lexer.l
	$(CXX) -c lex.yy.c $(CFLAGS)    

main.o: main.cpp Searcher.h Indexer.h TextParser.h Stem.h WordsBor.h DocumentsPool.h
	$(CXX) -c main.cpp $(CFLAGS) $(BOOST)   

clean:
	rm -f *.gch 
