%{

#include "hoc.h"
#include <math.h>

void yyerror (char *s);
int yylex ();
void warning(char *s, char *t);
void execerror(char *s, char *t);
void fpecatch();
extern double Pow(double, double);


%}
%union {
	double val;
	Symbol *sym;//int index;
}

%token <val> NUMBER
%token <sym> VAR BLTIN INDEF
%type <val> expr asgn

%right '='
%left '+' '-'
%left '*' '/'
%left UNARYMINUS
%right '^'

%% /* A continuación las reglas gramaticales y las acciones */
list:   
	| list '\n'
	| list asgn '\n'
	| list expr '\n'  { printf("\t%.8g\n", $2); }
	| list error '\n' { yyerrok; } 
	;
asgn:	VAR '=' expr { 
                  $$ = $1->u.val = $3; //$$=mem[$1]=$3;
                  $1->type=VAR; }
	;
expr:      NUMBER { $$ = $1;  }
	| VAR { if($1->type == INDEF)
			execerror("variable no definida", $1->name);
		$$=$1->u.val;//$$=mem[$1];
	      }
	| asgn
	| BLTIN  '(' expr ')' { $$=($1->u.ptr)($3);}
        | expr '+' expr     { $$ = $1+$3;  }
        | expr '-' expr     { $$ = $1-$3;  }
        | expr '*' expr     { $$ = $1*$3;  }
        | expr '/' expr     { 
		if($3==0.0)
			execerror("division por cero", "");
		$$ = $1/$3; }
	| expr '^' expr     { $$=Pow($1, $3);}
        | '(' expr ')'      { $$ = $2;}
	| '-' expr %prec UNARYMINUS { $$= -$2; }
	;
%%

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>

jmp_buf begin;

char *progname;
int lineno = 1;

void main (int argc, char *argv[]){
	progname=argv[0];
	init();
	setjmp(begin);
	signal(SIGFPE, fpecatch);
  	yyparse ();
}

void execerror(char *s, char *t){
	warning(s, t);
	longjmp(begin, 0);
        //puts(s);
}

void fpecatch(){
	execerror("excepcion de punto flotante", (char *)0);
}

int yylex (){
  	int c;

  	while ((c = getchar ()) == ' ' || c == '\t')  
  		;
 	if (c == EOF)                            
    		return 0;
  	if (c == '.' || isdigit (c)){//num deci
      		ungetc (c, stdin);
      		scanf ("%lf", &yylval.val);
                //puts("NUMBER");
	      	return NUMBER;
    	}
	if(isalpha(c)){//ID ( VAR, BLTIN, INDEF) 
		Symbol *s;
		char sbuf[10000], *p=sbuf;
		do {
			*p++=c;
		} while ((c=getchar())!=EOF && isalnum(c));
		ungetc(c, stdin);
		*p='\0';      
		if((s=lookup(sbuf))==(Symbol *)NULL)
			s=install(sbuf, INDEF, 0.0);
		yylval.sym=s;   
                if(s->type == INDEF){
			return VAR;
                } else {
 		//printf("func=(%s) tipo=(%d) \n", s->name, s->type);
                        return s->type;//VAR o BLTIN
                }
	}
  	if(c == '\n'){
                //puts("enter");
		lineno++;
        }
        /*if( c== '(')
		puts("(");
        if( c== ')')
		puts(")");*/
  	return c;                                
}

void yyerror (char *s)  /* Llamada por yyparse ante un error */
{
	warning(s, (char *) 0);
}

void warning(char *s, char *t)
{
	fprintf (stderr, "%s: %s", progname, s);
	if(t)
		fprintf (stderr, " %s", t);
	fprintf (stderr, "cerca de la linea %d\n", lineno);
}
