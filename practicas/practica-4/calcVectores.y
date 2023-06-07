
%{
#include "symbol.h"
void yyerror (char *s);
int yylex();
void warning(char *s, char *t);
#define code2(c1, c2)     code(c1); code(c2);
#define code3(c1, c2, c3) code(c1); code(c2); code(c3);
%}
%union {
	Symbol *sym;
	Inst *inst;
}

%token<sym> NUMBER VAR VARVECTOR VARESCALAR INDEF
%left '+' '-'
%left "*"
%left 'x'

%% 
list:   
   | list '\n'
   | list asgnVector '\n'     { code2(printVector, STOP); return 1; }
   | list asgnEscalar '\n'    { code2(printEscalar, STOP); return 1; } 
   | list expVectorial '\n'   { code2(printVector, STOP); return 1; }
   | list expEscalar '\n'     { code2(printEscalar, STOP); return 1; } 
   ;
asgnVector: VAR '=' expVectorial          { code3(varpush, (Inst)$1, assignVector); } //Declaracion
          | VARESCALAR '=' expVectorial   { code3(varpush, (Inst)$1, assignVector); } //Redefiniciones
          | VARVECTOR '=' expVectorial    { code3(varpush, (Inst)$1, assignVector); }
          ;
asgnEscalar: '#' VAR '=' expEscalar          { code3(varpush, (Inst)$2, assignEscalar); } //Declaracion
           | '#' VARESCALAR '=' expEscalar   { code3(varpush, (Inst)$2, assignEscalar); }  //Redefeniciones
           | '#' VARVECTOR '='expEscalar     { code3(varpush, (Inst)$2, assignEscalar); } 
           ;          
expVectorial: vector                   
   | VAR                               { code3(varpush, (Inst)$1, evalVector); }
   | VARVECTOR                         { code3(varpush, (Inst)$1, evalVector); }
   | expVectorial '+' expVectorial     { code(add); }
   | expVectorial '-' expVectorial     { code(sub); }
   | expVectorial 'x' expVectorial     { code(cross); }
   | expEscalar '*' expVectorial       { code(multiEscalarVect); }
   | expVectorial '*' expEscalar       { code(multiVectEscalar); }
   | '(' expVectorial ')'              
   ;
expEscalar: NUMBER                     { code2(escalarpush, (Inst)$1); }
   |  VARESCALAR                       { code3(varpush, (Inst)$1, evalEscalar); }
   | '|' expVectorial '|'              { code(magnitude); }
   | '|' expEscalar '|'                
   | expVectorial '*' expVectorial     { code(point); }
   | '(' expEscalar ')'                
   ;
vector: '[' listnum ']' { code(vectorpush); }
   ;
listnum: 
   | NUMBER listnum { code2(numpush, (Inst)$1); }
   ;
%%
#include <stdio.h>
#include <ctype.h>
#include <setjmp.h>

jmp_buf begin;

int lineno = 1;

void init(){
   initConstants();
}

void main() { 
   init();
   setjmp(begin);
   for(initcode(); yyparse() ; initcode())
	   execute(prog);
}

//Analizador léxico
int yylex (){
  	int c;
        
  	while ((c = getchar ()) == ' ' || c == '\t');
 	if (c == EOF)                            
    		return 0;
  	if (c == '.' || isdigit (c)){
		double d;
      		ungetc (c, stdin);
      		scanf ("%lf", &d);
                yylval.sym=install("", NUMBER, d);

	      	return NUMBER;
    	}
	if(isalpha(c) && c!=120){
		Symbol *s;
		char sbuf[200], *p=sbuf;
		do {
			*p++=c;
		} while ((c=getchar())!=EOF && isalnum(c));
		ungetc(c, stdin);
		*p='\0';
		if((s=lookup(sbuf))==(Symbol *)NULL)
			s=install(sbuf, INDEF, 0.0);
		yylval.sym=s;
		return s->type == INDEF ? VAR : s->type;
	}
  	if(c == '\n'){
		lineno++;
   }
  	return c;                                
}

void yyerror(char* s) { 
  printf("\x1b[31m%s\n\x1b[0m", s); 
  longjmp(begin, 0);
  return ; 
}