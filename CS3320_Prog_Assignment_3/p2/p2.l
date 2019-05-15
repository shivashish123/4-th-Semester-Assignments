%{
#include "y.tab.h"
void yyerror (const char *s);
int yylex();
char buffer[8];
%}
%%
[#](.)*\n							{;}					/* eating the comments */  
[I|i][N|n][I|i][T|t]				{return INIT;}	
[I|i][F|f]							{return IF;}
[E|e][L|l][S|s][E|e]				{return ELSE;}
[S|s][E|e][L|l][E|e][C|c][T|t]		{return SELECT;}
[D|d][E|e][L|l][A|a][Y|y]			{return DELAY;}
[R|r][E|e][A|a][D|d][I|i][N|n][T|t] {return READINT;}
[I|i][N|n][T|t]						{return INT;}
[ ]									{return space;} 	/* space character */
[\n]								{return newline;}	/* newline */
[:=]								{return yytext[0];}
[=|>|<|!][=]						{strcpy(yylval.name ,(char*)yytext); return OPERATOR;} /* >= <= != == */
[>|<]								{strcpy(yylval.name ,(char*)yytext); return OPERATOR;}	/* > < */
[a-zA-Z]+							{strcpy(yylval.name ,(char*)yytext); return IDENTIFIER;}

[\t][01][\t][01][\t][01][\t][01][\t][01][\t][01][\t][01][\t][01]  {buffer[0] = yytext[1]; buffer[1] = yytext[3];buffer[2] = yytext[5];
									buffer[3] = yytext[7];buffer[4] = yytext[9];buffer[5] = yytext[11];
									buffer[6] = yytext[13];buffer[7] = yytext[15];
									strcpy(yylval.name,buffer);return binary;} /* Removing the tab characters and storing it in buffer and then returning it to yylval.name */ 
[\t]								{return tab;}	/* tab character */
[0-9]+								{strcpy(yylval.name ,(char*)yytext); return NUMBER;}
%%
int yywrap (void) {return 1;}