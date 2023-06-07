
%{
#include "symbol.h"
void yyerror (char *s);
int yylex();
void warning(char *s, char *t);
#define code2(c1, c2)     code(c1); code(c2);
#define code3(c1, c2, c3) code(c1); code(c2); code(c3);
int indef;
%}
%union {
	Symbol *sym;
	Inst *inst;
   int narg;      /*   numero de argumentos */
}

%token<sym> NUMBER PRINT VAR VARVECTOR VARESCALAR INDEF WHILE IF ELSE FOR
%token<sym>   FUNCTION PROCEDURE RETURN FUNC PROC
%token<narg>  ARG
%type<inst> stmt asgnVector asgnEscalar expVectorial expEscalar stmtlist cond while if end for condfor stmtfor
%type<inst> begin
%type<sym>  procname
%type<narg> arglist

%right '='
%left	OR
%left	AND
%left	GT GE LT LE EQ NE
%left '+' '-'
%left "*"
%left 'x'
%left ';'
%left	NOT

%% 
list:   
   | list '\n'
   | list asgnVector '\n'     { code2(printVector, STOP); return 1; }
   | list asgnEscalar '\n'    { code2(printEscalar, STOP); return 1; } 
   | list expVectorial '\n'   { code2(printVector, STOP); return 1; }
   | list expEscalar '\n'     { code2(printEscalar, STOP); return 1; } 
   | list defn '\n'
   | list stmt '\n'	{ code(STOP); return 1; }
   ;
asgnVector: VAR '=' expVectorial          { $$=$3; code3(varpush, (Inst)$1, assignVector); } //Declaracion
          | VARESCALAR '=' expVectorial   { $$=$3; code3(varpush, (Inst)$1, assignVector); } //Redefiniciones
          | VARVECTOR '=' expVectorial    { $$=$3; code3(varpush, (Inst)$1, assignVector); }
          | ARG '=' expVectorial         { defnonly("$"); code2(argassign,(Inst)$1); $$=$3;}
          ;
asgnEscalar: '#' VAR '=' expEscalar          { $$=$4; code3(varpush, (Inst)$2, assignEscalar); } //Declaracion
           | '#' VARESCALAR '=' expEscalar   { $$=$4; code3(varpush, (Inst)$2, assignEscalar); }  //Redefeniciones
           | '#' VARVECTOR '='expEscalar     { $$=$4; code3(varpush, (Inst)$2, assignEscalar); } 
           ;          
expVectorial: vector                   
   | VAR                               { $$=code3(varpush, (Inst)$1, evalVector); }
   | VARVECTOR                         { $$=code3(varpush, (Inst)$1, evalVector); }
   | ARG                               {   defnonly("$"); $$ = code2(arg, (Inst)$1); }
   | FUNCTION begin '(' arglist ')'    { $$ = $2; code3(call,(Inst)$1,(Inst)$4); }  
   | expVectorial '+' expVectorial     { code(add); }
   | expVectorial '-' expVectorial     { code(sub); }
   | expVectorial 'x' expVectorial     { code(cross); }
   | expEscalar '*' expVectorial       { code(multiEscalarVect); }
   | expVectorial '*' expEscalar       { code(multiVectEscalar); }
   | '(' expVectorial ')'              {$$ = $2;}
   ;
expEscalar: NUMBER                     { $$ = code2(escalarpush, (Inst)$1); }
   |  VARESCALAR                       { $$ = code3(varpush, (Inst)$1, evalEscalar); }
   | '|' expVectorial '|'              { $$ = code(magnitude); }
   | '|' expEscalar '|'                { $$ = $2; }
   | expVectorial '*' expVectorial     { code(point); }
   | '(' expEscalar ')'                { $$ = $2; }
   //ESCALAR-ESCALAR 
   | expEscalar GT expEscalar          { code(gtEE); } 
	| expEscalar GE expEscalar          { code(geEE); }
	| expEscalar LT expEscalar          { code(ltEE); }
	| expEscalar LE expEscalar          { code(leEE); }
	| expEscalar EQ expEscalar          { code(eqEE); }
	| expEscalar NE expEscalar          { code(neEE); }
	| expEscalar AND expEscalar         { code(andEE); }
	| expEscalar OR expEscalar          { code(orEE); }
	| NOT expEscalar                    { $$ = $2; code(notE); } 
   //VECTOR-VECTOR
   | expVectorial GT expVectorial      { code(gtVV); }
	| expVectorial GE expVectorial      { code(geVV); }
	| expVectorial LT expVectorial      { code(ltVV); }
	| expVectorial LE expVectorial      { code(leVV); }
	| expVectorial EQ expVectorial      { code(eqVV); }
	| expVectorial NE expVectorial      { code(neVV); }
	| expVectorial AND expVectorial     { code(andVV); }
	| expVectorial OR expVectorial      { code(orVV); }
	| NOT expVectorial                  { $$ = $2; code(notV); }
   //ESCALAR-VECTOR
   | expEscalar GT expVectorial      { code(gtEV); }
	| expEscalar GE expVectorial      { code(geEV); }
	| expEscalar LT expVectorial      { code(ltEV); }
	| expEscalar LE expVectorial      { code(leEV); }
	| expEscalar EQ expVectorial      { code(eqEV); }
	| expEscalar NE expVectorial      { code(neEV); }
	| expEscalar AND expVectorial     { code(andEV); }
	| expEscalar OR expVectorial      { code(orEV); }
   //VECTOR-ESCALAR
   | expVectorial GT expEscalar      { code(gtVE); }
	| expVectorial GE expEscalar      { code(geVE); }
	| expVectorial LT expEscalar      { code(ltVE); }
	| expVectorial LE expEscalar      { code(leVE); }
	| expVectorial EQ expEscalar      { code(eqVE); }
	| expVectorial NE expEscalar      { code(neVE); }
	| expVectorial AND expEscalar     { code(andVE); }
	| expVectorial OR expEscalar      { code(orVE); }
   ;
vector: '[' listnum ']' { code(vectorpush); }
   ;
listnum: 
   | NUMBER listnum { code2(numpush, (Inst)$1); }
   ;
stmt:   asgnEscalar                    { code(pop1); code(STOP); }
   | asgnVector                        { code(pop1); code(STOP); }
	| PRINT expEscalar                  { code(prexpresc); $$ = $2; code(STOP);}
	| PRINT expVectorial                { code(prexprvec); $$ = $2; code(STOP);}
	| while cond stmt end               { ($1)[1] = (Inst)$3;     /* cuerpo de la iteracion */ 
		                                   ($1)[2] = (Inst)$4; }   /* terminar si la condicion no se cumple */
	| if cond stmt end                  {    /* proposicion if que no emplea else */ 
		                                   ($1)[1] = (Inst)$3;     /* parte then */ 
		                                   ($1)[3] = (Inst)$4;
                                       }   /* terminar si la condicion no se cumple */ 
	| if cond stmt end ELSE stmt end    {  /* proposicion if con parte else */
                                         ($1)[1]   =   (Inst)$3;	/*  parte then  */
                                         ($1)[2]   =   (Inst)$6;	/* parte else   */
                                         ($1)[3]   =   (Inst)$7;   } /*   terminar si la condicion no se cumple  */
   | for '(' stmtfor ';' condfor ';' stmtfor ')' stmt end   {
                                                               ($1)[1] = (Inst)$5;  /* condicion del for */  
                                                               ($1)[2] = (Inst)$7; /* stmtfor que se repite cada ciclo */
                                                               ($1)[3] = (Inst)$9; /*   cuerpo de la iteracion  */ 
                                                               ($1)[4] = (Inst)$10; /*   terminar si la condicion no se cumple  */
                                                            }                                  
	|   '{'   stmtlist   '}'                  { $$  =  $2; }
   | RETURN                                  { defnonly("return"); code(procret); } 
   | RETURN expVectorial                     { defnonly( "return" ); $$ = $2; code(funcret) ; } 
   | PROCEDURE begin '(' arglist ')'         { $$ = $2; code3(call, (Inst)$1, (Inst)$4); } 
   ;
cond:	'('   expEscalar   ')'           { code(STOP);  $$  =  $2; }
   ;
condfor: expEscalar      { code(STOP);  $$  =  $1; }
   ;
stmtfor: asgnEscalar                       { code(pop1); code(STOP); }
   | asgnVector                        { code(pop1); code(STOP); }
	| PRINT expEscalar                  { code(prexpresc); $$ = $2; code(STOP);}
	| PRINT expVectorial                { code(prexprvec); $$ = $2; code(STOP);}
   ;
while:	WHILE                         { $$ = code3(whilecode,STOP,STOP); }
	;
if:     IF                             { $$=code(ifcode); code3(STOP, STOP, STOP); }
	;
for:    FOR                            { $$ = code(forcode); code3(STOP, STOP, STOP); code(STOP);   }
   ;   
begin:  /* nada */          { $$ = progp; }   
end:     /* nada */                    { code(STOP); $$ = progp; }
	;
stmtlist: /* nada */	                  { $$ = progp; }
	| stmtlist '\n' 
	| stmtlist stmt
	;    // $1   $2        $3
defn:    FUNC procname { $2->type=FUNCTION; indef=1; }
//$4   $5  $2
  '(' ')' stmt { code(procret); define($2); indef=0; } 
   | PROC procname { $2->type=PROCEDURE; indef=1; }
'(' ')' stmt { code(procret); define($2); indef=0; }
   ;
procname: VAR
   | FUNCTION 
   | PROCEDURE
   ;
arglist:  /* nada */      { $$ = 0; }
   | expVectorial                 { $$ = 1; }
   | arglist ',' expVectorial     { $$ = $1 + 1; }
   ;

%%
#include <stdio.h>
#include <ctype.h>
#include <setjmp.h>

char	*progname;
int 	lineno = 1;
jmp_buf  begin;
int 	indef;
char 	*infile;
FILE	*fin;
char	**gargv;
int 	gargc;
int 	c;/*global for use by warning()*/

int yylex (){
  	int c;
  	while ((c = getchar ()) == ' ' || c == '\t')  
  		/*SALTA BLANCOS*/;
 	if (c == EOF)                            
    	return 0;
  	if (c == '.' || isdigit(c)) {   /* numero */
      double d;
      ungetc(c, stdin); 
      scanf("%lf", &d);
      yylval.sym = install("", NUMBER, d);
      return NUMBER; 
   }
   if (isalpha(c) && c!=120) { 
      Symbol *s;
      char sbuf[100], *p = sbuf; 
      do {
         if (p >= sbuf + sizeof(sbuf) - 1) { 
            *p = '\0'; 
            printf("\x1b[31m%s\n\x1b[0m", "name too long"); 
         }
      *p++ = c;
      } while ((c=getc(stdin)) != EOF && isalnum(c)); 
      ungetc(c, stdin); 
      *p = '\0'; 
      if ((s=lookup(sbuf)) == 0)
               s=install(sbuf, INDEF, 0.0); 
      yylval.sym = s;
      return s->type == INDEF ? VAR : s->type;
   }
  	if(c == '\n')
		lineno++;
	if( c == '$'){ /*¿Argumento?*/
		int n = 0;
		while(isdigit(c=getc(fin)))
			n =  10 * n + c - '0';
		ungetc(c,fin);
		if(n == 0)
			execerror("strange $...", (char *)0);
		yylval.narg = n;
		return ARG;
	}
	switch (c){
	case '>':	return follow('=',GE,GT);
	case '<':	return follow('=',LE,LT);
	case '=':	return follow('=',EQ,'=');
	case '!':	return follow('=',NE,NOT);
	case '|':	return follow('|',OR,'|');
	case '&':	return follow('&',AND,'&');
	case '\n':	lineno++; return '\n';
	default:	return c;
	}
  	return c;                                
}

follow (expect,ifyes,ifno)
{
	int c = getchar();

	if(c == expect)
		return ifyes;
	ungetc(c, stdin);
	return ifno;
}

void defnonly(char *s)	/*advertencia en caso de definición ilegal*/
{
	if(!indef)
      printf("\x1b[31m%s\n\x1b[0m", "used outside definition"); 

}

void run() /* ejecutar hasta al fin de archivo (EOF)*/
{
	setjmp(begin);
	for(initcode();yyparse();initcode())
		execute(progbase);
}

int moreinput()
{
	if(gargc--<=0)
		return 0;
	if(fin && fin !=stdin)
		fclose(fin);
	infile = *gargv++;
	lineno = 1;
	if(strcmp(infile,"-") == 0){
		fin = stdin;
		infile = 0;
	}else if((fin =fopen(infile,"r")) ==NULL){
		fprintf(stderr,"%s: cannot open %s\n",progname,infile);
		return moreinput();
	}
	return 1;
}

int main (int argc, char *argv[]){	
   int i;
   progname = argv[0];
   if(argc == 1){	/*fake an argument list*/
	static char *stdinonly[] = { "-"};
	gargv = stdinonly;
	gargc = 1;
   }else{
	gargv = argv +1;
	gargc = argc -1;
   }
   initConstants();
   while(moreinput())
	run();
   return 0;
}

void execerror(char *s, char *t){
	printf("\x1b[31m%s %s\n\x1b[0m", s, t); 
	longjmp(begin, 0);
}

void yyerror(char* s) { 
  printf("\x1b[31m%s\n\x1b[0m", s); 
  longjmp(begin, 0);
  return ; 
}
