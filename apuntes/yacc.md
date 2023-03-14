# YACC

- [YACC](#yacc)
  - [Glosario de Términos](#glosario-de-términos)


- _Yet Another Compiler Compiler_.
- Durante los años 70 estaba de moda hacer compiladores de compiladores.
- Nosotros le damos un archivo a YACC con extensión `.y` con lo que nos genera un código en C.
- Es un generador de analizadores sintácticos.
- Todo lo que comienza con `YY` pertenece a YACC.
- Partes del archivo 
  - Declaraciones.
  - Reglas: se parece a un **esquema de traducción**.
  - Soporte.
- El código en C está entre llaves {}.
- En YACC no se llaman _acciones semánticas_, se llaman _acciones gramaticales_.

Proceso de ejecución de YACC:

1. `.y` lo compila YACC.
2. `ytab.c` lo compila C.
3. `.exe` lo ejecuta el S.O.

Código de ejemplo:

```yacc
/* DECLARACIÓN */
%{
  /* YY Stack Type = double */
  /* Le dice el tipo de datos de los elementos de la pila */
  #define YYSTYPE double
%}
%token NUMBER
%left '+''-'
%left '*''/'
%%
/* CÓDIGO */
list: /* nada */
  | list '\n'
  | list exp'\n'{printf("it%.8g\n", $2);}
  ;
exp: NUMBER
  | exp{$$=$1} /* $1 = lexema de NUMBER */
  | exp'+'exp{$$=$1+$3;} /* en este caso: $1=exp (la evaluación de la expresión que está inmediatamente a la izquierda del +), $2='+', $3=exp */
  | exp'-'exp{$$=$1-$3;}
  | exp'*'exp{$$=$1*$3;}
  | exp'/'exp{$$=$1/$3;}
  |'('exp')'{$$=$2;}
%%

/* CÓDIGO DE SOPORTE */
void main()
{
  yyparse();
}

int yylex()
{
  int c;
  /* salta blancos */
  while((c = getchar()) == ' ' || c == '\t')
  {
    /* enunciado nulo */;
  }
  /* en este punto c tiene un caracter que no es espacio ni tabulador */
  if (c == EOF)
  {
    /* es para avisarle al analizador sintáctico que ya no hay más tokens */
    return 0;
  }
  if (c == '.' || isdigit(c))
  {
    getc(c, stdin);
    scanf("%.lf", &yylval); /* yylval es una variable global */
    return NUMBER;
  }
  return c;
}

void yyerror(char *s)
{
  puts(s);
}
```

- Defectos de la gramática:
  - Es recursiva por la izquierda.
  - Es ambigua.
- `yyparse()` es el analizador sintáctico.
- YACC elimina la ambigüedad indicando la precedencia y la asociatividad.
- En una gramática libre de contexto del lado derecho tenemos un no terminal y del lado izquierdo una cadena de símbolos gramaticales.
- `$1` hace referencia al primer símbolo gramatical del lado derecho de la producción.
- `$2` hace referencia al segundo símbolo gramatical del lado derecho de la producción.
- `$3` hace referencia al tercer símbolo gramatical del lado derecho de la producción.
- `$n` hace referencia al n-éscimo símbolo gramatical del lado derecho de la producción.
- `NUMBER` es un token, un tipo de token.
- `#define YYSTYPE`: Le dice el tipo de datos de los elementos de la pila.
- `$$` es dónde se guarda el valor que se asigna a toda la producción.


## Glosario de Términos

YACC
: Yet Another Compiler Compiler.

Macro
: Algo que se va a reemplazar.

Acción Gramatical
: Acción Semántica

Acción Semántica
: Fragmento del programa que se encuentra intercalado en el lado derecho de una 
producción.

Asociatividad
: Reglas para reducir expresiones y poder evaluarla.

Gramática Ambigua
: Una gramática es ambigua si existe una cadena que pertenece al lenguaje generado
por la gramática para la cual se pueden dibujar dos AAS diferentes.
: Pueden existir dos evaluaciones que puedan ser distintas para la misma cadena.

`;` en C, solito
: Es un enunciado nulo.

Componente Léxico
: Conjunto de caracteres con significado colectivo.

Lexema
: Caracteres en particular que forman un token.
: Por ejemplo en el token `223` el lexema es `2`, `2` y `3`.