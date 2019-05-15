%{

#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
#include <fstream>
void yyerror (const char *s);
int yylex();
%}

%union {char name[5000];}         	/* Yacc definitions */
%start statement					/* start symbol */ 
%token INIT 						/* keywords */
%token IF
%token ELSE
%token SELECT
%token DELAY
%token READINT
%token INT
%token <name> NUMBER			 	/* number is represented as sequence of (0-9) characters */
%token <name> IDENTIFIER 			/* identifier is a sequence of letters */
%token <name> OPERATOR				/* comparison operators (>, <, >=, <=, ==, !=) */
%token newline						/* token for newline(\n) */
%token space						/* token for space( ) */
%token tab							/* token for tab(\t) character */
%token <name> binary				/* sequence of 0s, 1s representing value a-g and dp in 7 segment display*/
%type <name> statement line

%%

statement : line 
		  | statement line			/* multi-line statements */
		  ;
line : INIT newline												{ printf("\tinit();\n\twhile(1)\n\t{\n"); }
	 | SELECT ':' space NUMBER newline 							{ printf("\t\tselect(%s);\n",$4); } 
	 | binary newline 											{ printf("\t\twrite(strtol(\"%s\"));\n",$1); }
	 | IDENTIFIER ':' space INT newline							{ printf("\t\tint %s;\n",$1); }
	 | IF space IDENTIFIER space OPERATOR space NUMBER ':' newline binary newline											{ printf("\t\tif(%s%s%s)\n\t\t{\n\t\t\twrite(strtol(\"%s\"));\n\t\t}\n",$3,$5,$7,$10); }
	 | ELSE ':' newline binary newline																						{ printf("\t\telse\n\t\t{\n\t\t\twrite(strtol(\"%s\"));\n\t\t}\n",$4); }
	 | DELAY ':' space NUMBER newline 							{ printf("\t\tdelay(%s);\n",$4); } 
	 | IDENTIFIER space '=' space READINT newline				{ printf("\t\tinput = readInt();\n"); } 
	 | DELAY ':' space NUMBER 									{ printf("\t\tdelay(%s);\n",$4); } 
	 ;

%%

int main()
{
	/* printing header files and main() */
	printf("#include<stdio.h>\n#include<stdlib.h>\n#include<seven_segment.h>\nint main()\n{\n");
	yyparse();
	printf("\t}\n");
	printf("}\n");

}

void yyerror (const char *s) {fprintf (stderr, "%s\n", s);} 