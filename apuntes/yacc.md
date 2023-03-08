# YACC

- _Yet Another Compiler Compiler_.
- Durante los años 70 estaba de moda hacer compiladores de compiladores.
- Nosotros le damos un archivo a YACC con extensión `.y` con lo que nos genera
- Es un generador de analizadores léxicos.
un código en C.
- Partes del archivo 
  - Declaraciones.
  - Reglas: se parece a un esquema de traducción.
  - Soporte.

Proceso de ejecución de YACC:

1. `.y` lo compila YACC.
2. `ytab.c` lo compila C.
3. `.exe` lo ejecuta el S.O.

Código de ejemplo:

```yacc
/* DECLARACIÓN */
%{
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
  | exp'+'exp{$$=$1+$3;}
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
  while((c = getchar()) == ' ' || c == 'it')
  {
    /* vacío */;
  }
  if (c == EOF)
  {
    return 0;
  }
  if (c == '.' || isdigit(c))
  {
    wgetc(c, stdin);
    scanf("%.lf", &yylval);
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

## Glosario de Términos

YACC
: Yet Another Compiler Compiler.

Macro
: Algo que se va a reemplazar.
