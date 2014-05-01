%{
#include <stdio.h>
#include <iostream>
#include <string>
#include "indexer.h"
%}

%union {
    char word[256];
    char endOfSentence[64];
}

%token MISTER
%token MISSIS
%token ELLIPSIS
%token NOTENDL
%token<word> WORD
%token<endOfSentence> ENDL

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
    | ELLIPSIS unexpected
    | '\n' unexpected
    ;

endl
    : '.'           
    | '!'           
    | '?'           
    | ELLIPSIS      
    | ENDL         
    | '\n'
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


