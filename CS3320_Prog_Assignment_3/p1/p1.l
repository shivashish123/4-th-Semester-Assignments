%{
#include "y.tab.h"
void yyerror (const char *s);
int yylex();
%}
%%
[K|Q|R|B|N]	{yylval.name = yytext[0]; return piece;}	/* chess pieces * /
/*it gives the position by converting it into a number i.e [2][3] will be given as (3-1)*8 + (2-1) */
[a-h][1-8]	{yylval.num = ((yytext[1]-'1')*8+(yytext[0]-'a'));return position;}	
[\n]		{return newline;}
[ \t]       {;}
[0-9]+["."]	{;}	/* eat the numbers used for listing preceding before move */
[=]			{return yytext[0];}
[-x]		{return yytext[0];}

%%
int yywrap (void) {return 1;}
