%{
// Librerías base.
#include <stdio.h>
#include <math.h>

// Para los prototipos de funciones y estructuras
#include "complejo_calc.h"

// El tipo de la pila será ComplejoAP
#define YYSTYPE ComplejoAP

// Prototipos de funciones
void yyerror (char *s);
int yylex ();
void warning(char *s, char *t);
%}

%token CNUMBER
%left '+' '-'
%left '*' '/'

%% 

list:   
  | list'\n'
        | list exp '\n'  { imprimirC($2); }
  ;
exp:    CNUMBER           { $$ = $1;  }
        | exp '+' exp     { $$ = Complejo_add($1,$3);  }
        | exp '-' exp     { $$ = Complejo_sub($1,$3);  }
        | exp '*' exp     { $$ = Complejo_mul($1,$3);  }
        | exp '/' exp     { $$ = Complejo_div($1,$3);  }
        | '(' exp ')'     { $$ = $2;}
  ;

%%

// Variables globales.
char *progname; // Nombre del programa.
int lineno = 1; // Línea de entrada dónde se encontró un error.

// Función principal.
int main (int argc, char *argv[]){
  progname=argv[0];
  yyparse ();
  return 0;
}

// Función para captar todos los errores por poarte de YACC
void yyerror (char *s) {
  warning(s, (char *) 0);
}

// Función que define cómo imprimir los errores de YACC.
void warning(char *s, char *t){
  fprintf (stderr, "%s: %s", progname, s);
  if(t)
    fprintf (stderr, " %s", t);
  fprintf (stderr, "cerca de la linea %d\n", lineno);
}



