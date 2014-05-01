%{
#include <stdio.h>
#include <stdlib.h>
#include "Indexer.h"
#include <string.h>
%}


NUMERAL ([1-9][0-9]*)|0
WORD [A-Za-z'\'']*

%option noyywrap
%option yylineno

%%
"\""        { Indexer::Instance().GetParser().NewItem("\"");} 
"Mr."       { Indexer::Instance().GetParser().NewItem("Mr."); return MISTER;}
"Mrs."      { Indexer::Instance().GetParser().NewItem("Mrs."); return MISSIS;}
"..."       { Indexer::Instance().GetParser().NewItem("..."); return ELLIPSIS;}
"-\""       { Indexer::Instance().GetParser().NewItem("-\""); /*return ENDL;*/}
"?\""       { Indexer::Instance().GetParser().NewItem("?\"");}
"!\""       { Indexer::Instance().GetParser().NewItem("!\"");}
","         { Indexer::Instance().GetParser().NewItem(",");}
"."         { Indexer::Instance().GetParser().NewItem("."); return '.';}
"("         { Indexer::Instance().GetParser().NewItem("(");}
")"         { Indexer::Instance().GetParser().NewItem(")");}
"?"         { Indexer::Instance().GetParser().NewItem("?"); return '?';}
"!"         { Indexer::Instance().GetParser().NewItem("!"); return '!';}
";"         { Indexer::Instance().GetParser().NewItem(";");}
":"         { Indexer::Instance().GetParser().NewItem(":");}
"-"         { Indexer::Instance().GetParser().NewItem("-");}
{WORD}      { Indexer::Instance().GetParser().NewItem(yytext); strcpy(yylval.word, yytext); return WORD;}
{NUMERAL}   {Indexer::Instance().GetParser().NewItem(yytext); strcpy(yylval.word, yytext); return WORD;}
" "         {Indexer::Instance().GetParser().NewItem(" ");}
"\n"        {return '\n';}
"*"         { /* do nothing */}
"\t"        {Indexer::Instance().GetParser().NewItem("\t");}
. 		{ printf("Lexical error: symbol '%c'!\n", yytext[0]); exit(0); }
<<EOF>> { return 0; /*static int once = 0; if (once++) return 0; else return '\n';*/ }
%%

