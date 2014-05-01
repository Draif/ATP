%{
#include <stdio.h>
#include <iostream>
#include <string>
#include "Indexer.h"
%}

%union {
    char word[256];
    char endOfSentence[64];
}

%token MISTER
%token MISSIS
%token NOTENDL
%token<word> WORD
%token<endOfSentence> ENDL
%token UNRECOGNIZED

%%
program
    : sentence 
    ;
        
sentence
    : /* epsilon */
    | new_sentence sentence 
    ;

new_sentence
    : unexpected bowels endl {Indexer::Instance().GetParser().Sentence();}
    ;

unexpected
    : /* epsilon */
    | endl unexpected
    ;

endl
    : ENDL          
    | '\n' 
    | UNRECOGNIZED 
    ;

bowels
    : item next_word 
    ;

next_word
    : /* epsilon */
    | bowels
    ;

item
    : WORD      {Indexer::Instance().IndexWord($1);}
    | MISTER    {Indexer::Instance().IndexWord("Mr");}
    | MISSIS    {Indexer::Instance().IndexWord("Mrs");}
    ;


