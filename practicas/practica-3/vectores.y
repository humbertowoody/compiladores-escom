%{
#include "vectores.h"

// Prototipos de funciones.
void yyerror (char *s);
int yylex ();
void warning(char *s, char *t);
void execerror(char *s, char *t);
void fpecatch();

%}

// Unión con los tipos de datos para la pila de YACC.
%union{
    Vector *vector;
    double numero;
    Symbol *sym;
}

// Definición de tokens.
%token <numero> NUMBER // Número.
%token <sym> VAR INDEF // Variable.

// Definción de tipos.
%type <vector> exp vector component asgn
%type <numero> escalar

// Precedencia de operadores.
%right '='         // Se evalúa de derecha a izquierda.
%left '+' '-' ','  // Se evalúa de izquierda a derecha.
%left '*' 'X'      // Se evalúa de izquierda a derecha.
%left UNARYMINUS   // Se evalúa de izquierda a derecha.
%%
// Lista de entradas (entrada)
list:                                             
  | list          '\n'                            // Cuando es solo un enter.
  | list exp      '\n'  { imprimeVector($2); }    // Cuando es una expresión vectorial.
  | list asgn     '\n'  { imprimeVector($2); }    // Cuando es una asignación vectorial a = (1,2)
  | list escalar  '\n'  { printf("%.2f\n", $2); } // Cuando es una expresión escalar 1.2
  ;

// Asignación de variables.
asgn: 
  | VAR '=' exp {$$ = $1->u.val = $3; $1->type=VAR; }
  ;

// Operaciones con escalares.
escalar:
  | exp '*' exp                 {$$ = productoPunto($1, $3);}
  | '|'exp'|'                   {$$ = magnitud($2);}
  | NUMBER	                    {$$ = $1;}
  | '-'NUMBER %prec UNARYMINUS  {$$ = -$2;}
  ;

// Evaluación de expresiones.
exp: 
  | vector          { $$ = $1;  }
  | VAR             {  if($1->type == INDEF)execerror("Variable no definida, ", $1->name);
                          $$=$1->u.val;
                    }
  | asgn            { $$ = $1;  }
  | exp '+' exp     { $$ = sumaVector  ($1, $3);  }
  | exp '-' exp     { $$ = restaVector ($1, $3);  }
  | exp 'X' exp     { $$ = productoCruz($1, $3);  }
  | '('exp')'       { $$ = $2; }
  | escalar '*' exp { $$ = productoEscalar($1, $3);  }
  | exp '*' escalar { $$ = productoEscalar($3, $1);  }
;

// Definición del vector.
vector:
  | '('component')' { $$ = $2;}
  ;

// Lista de números adentro del vector.
component:    
  | component','component { $$ = unirVectores($1, $3);}
  | NUMBER            { $$ = creaVector(1, $1); }
  ;

%%

// Librerías requeridas.
#include <stdio.h>   // Entrada y salida estándar.
#include <ctype.h>   // Funciones para caracteres.
#include <signal.h>  // Manejo de excepciones.
#include <setjmp.h>  // Manejo de excepciones.

// Constantes de operación.
#ifndef MAX_NOMBRE_VARIABLE
#define MAX_NOMBRE_VARIABLE 200
#endif

// Variables globales.
char *progname; // Nombre del programa.
int lineno = 1; // Número de línea.
jmp_buf begin;  // Punto de retorno.

int main (int argc, char *argv[]){
  // Obtenemos el nombre del binario.
  progname=argv[0];

  setjmp(begin);
  
  // Añadimos el listener para manejar excepciones de punto flotante.
  signal(SIGFPE, fpecatch);

  // Iniciamos el parser.
  yyparse();

  // Fin de ejecución.
  return 0;
}

// Función para manejo de errores.
void execerror(char *s, char *t){
    warning(s, t);
    longjmp(begin, 0);
}

// Función para atrapar errores de punto flotante.
void fpecatch(){
    execerror("Excepcion de punto flotante", (char *)0);
}

// Parser de YACC.
int yylex (){
  // Variables locales.
  int c;          // Caracter leído.
  Symbol *s;      // Apuntador a símbolo para variables en la tabla de símbolos.
  char sbuf[MAX_NOMBRE_VARIABLE], // Búffer de entrada para almacenar el nombre de la variable.
      *p=sbuf;    // Apuntador al búffer de entrada.

  // Ciclo para ignorar tabuladores y espacios.
  while ((c = getchar ()) == ' ' || c == '\t')  
    ;

  // Verificamos si es EOF.
  if (c == EOF)
  {
      return 0;
  }
  
  // Verificamos si es un punto o un dígito.
  if (c == '.' || isdigit (c)) {
    // Regresamos el caracter a la entrada.
    ungetc (c, stdin);

    // Leemos el número. en formato de double.
    scanf ("%lf", &(yylval.numero));

    // Regresamos el número (token)
    return NUMBER;
  }

  // Si es un alfanúmerico y no es 'X' entonces es una variable.
  // Nota: 'X' es el operador que definimos para producto cruz.
  if(isalpha(c) && c!='X'){
    // Ciclo para copiar el nombre de la variable en P.
    do {
      *p++=c;
    } while ((c=getchar())!=EOF && isalnum(c) && c!='X');

    // Regresamos el caracter a la entrada.
    ungetc(c, stdin);

    // Agregamos el EOF.
    *p='\0';

    // Buscamos la variable en la tabla de símbolos.
    if((s=lookup(sbuf))==(Symbol *)NULL)
    {
      // Si devuelve nulo, entonces se guarda el nombre de la variable en la tabla de símbolos.
      s=install(sbuf, INDEF, (Vector *)NULL);
    }

    // Colcoamos la nueva entrada en yylval.
    yylval.sym=s;

    // Si el tipo de la entrada es INDEF, quiere decvir que acabamos de añadir 
    // una VAR(iable).
    if(s->type == INDEF){
      return VAR;
    } else {
      // Si es diferente a INDEF, si existía y solo regresamos el tipo encontrado.
      return s->type;
    }
  }

  // Si es un salto de línea, incrementamos el número de línea.
  if(c == '\n')
  {
    lineno++;
  }
  
  // Regresamos el caracter/token leído.
  return c;
}

/**
  * Función para imprimir errores de sintaxis.
  * @param s Mensaje de error.
  */
void yyerror (char *s) {
  warning(s, (char *) 0);
}

// Función para imprimir errores.
void warning(char *s, char *t){
  fprintf (stderr, "%s: %s", progname, s);
  if(t)
    fprintf (stderr, " %s", t);
  fprintf (stderr, "cerca de la linea %d\n", lineno);
}
