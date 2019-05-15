%{

#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
void yyerror (const char *s);
int yylex();
char chess[9][9];		/* char array used to store chess board configuration */
void update(char chesspiece,int position1, int position2);
%}

%union {int num; char name;}         /* Yacc definitions */
%start statement					/* start symbol */ 
%token <num> position 				/* token for chess piece position */ 
%token <name> piece        			/* token for type of chess piece */ 
%token newline
%type <name> statement line move promote

%%

statement : line 
		  | statement line /* multi-line statements */
	      ;

line  :	move promote newline 	/* Rule for promotion and moves */
      | move promote
      |	promote promote newline 	/* Rule for both promotion moves */
      | promote promote
      |	promote move newline
      | promote move
      | move move newline	 /* Rule for movement of black and white pieces from one position to another*/
	  | move move
      ;
promote : position '-' position '=' piece  {update($5,$1,$3);}	/* promotion */
	    ;
move  : piece position '-' position  { update($1,$2,$4);}	/* movement from one position to another */
	  | position '-' position	  { update('P',$1,$3);}
	  | piece position 'x' position  { update($1,$2,$4);}	/* capture move */
	  | position 'x' position	  { update('P',$1,$3);}
	  ;

%%

void update(char chesspiece,int position1, int position2)	/* updates given from and to positions */
{
	int row1 = (position1/8)+1;	/* obtain the row by dividing the given position by 8 */
	int row2 = (position2/8)+1;
	int col1 = (position1%8)+1; /* obtain the column by taking remainder with 8 */
	int col2 = (position2%8)+1;	/* adding 1 because it is 1-based indexing */
	chess[row1][col1] = ' ';
	chess[row2][col2] = chesspiece;
}

int main(){
	int i;

	/* Initialising the chess board */
	chess[0][0]=' ';
	for(i=1;i<=8;i++)
	{	
		chess[0][i] = (char)('a'+i-1);
		chess[i][0] = (char)('0'+i);
	}
	for(i=1;i<=8;i++)
	{
		chess[2][i]='P';
		chess[7][i]='P';
	}
	chess[1][1] = 'R';
	chess[1][2] = 'N';
	chess[1][3] = 'B';
	chess[1][4] = 'Q';
	chess[1][5] = 'K';
	chess[1][6] = 'B';
	chess[1][7] = 'N';
	chess[1][8] = 'R';

	chess[8][1] = 'R';
	chess[8][2] = 'N';
	chess[8][3] = 'B';
	chess[8][4] = 'Q';
	chess[8][5] = 'K';
	chess[8][6] = 'B';
	chess[8][7] = 'N';
	chess[8][8] = 'R';
	
	yyparse();	/* Parse the input file */
	
	/* Printing the final configuration on chess board */ 
	int j;
	for(i=0; i<9; i++)
	{
		for(j=0; j<9; j++)
		{
			printf("%c\t",chess[i][j]);
		}
		printf("\n");
		printf("\n");
	}
}

void yyerror (const char *s) {fprintf (stderr, "%s\n", s);} 



