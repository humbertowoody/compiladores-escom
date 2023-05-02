# YACC

- [YACC](#yacc)
    - [HOC2](#hoc2)
  - [Diferencias entre versiones de HOC](#diferencias-entre-versiones-de-hoc)
  - [HOC3](#hoc3)
  - [HOC4](#hoc4)
  - [HOC 6](#hoc-6)
  - [Glosario de Términos](#glosario-de-términos)

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
- `$n` hace referencia al n-ésimo símbolo gramatical del lado derecho de la producción.
- `NUMBER` es un token, un tipo de token.
- `#define YYSTYPE`: Le dice el tipo de datos de los elementos de la pila.
- `$$` es dónde se guarda el valor que se asigna a toda la producción.
- Cuando se hace referencia a un token, YACC nos devuelve un lexema.
- Para entender YACC hay que entender los Esquemas de Traducción.
- Lo que hace la definición dirigida por la sintaxis es traducir de una expresión
  a su evaluación, al resultado de evaluar esa expresión.

### HOC2

```yacc
%{
  double mem[26];
%}
%union {
  double val;
  int index;
}
%token val> NUMBER
%token index> VAR
%type val> exp /* es un no terminal */
%right '='
...
%left UNARYMINUS
%%
...
exp: NUMBER     { $$ = $1; }
  | VAR         { $$ = MEM[$1]; }
  | VAR '=' exp { $$ = mem[$1] = $3; }
  ...
  | '-' exp %prec UNARYMINUS {$$ = -$2;}
  ;
%%

int yylex() {
  if (c == '.' || isdigit(c))
  {
    ...
    scanf("%.ld", yylval.val);
    return NUMBER;
  }
  if (islower(c))
  {
    yylval.index = c - 'a';
    return VAR;
  }
  return c;
}
```

- Existen dos formas de definir el tipo de dato de la pila de YACC:
  1. Usando `#define YYSTYPE`
  2. Usando `%union` con lo que se declara una Unión de C.
- `exp` es un **no terminal**.
- `%type` es usado para indicarle qué parte de la unión usar en el no terminal.
- `yylex()` _trabaja para_ `yyparse()`.
- `yylex()` es el analizador léxico.
- `yyparse()` es el analizador sintáctico.
- `yylval` es una variable global.
- En este ejemplo `yylval` es de tipo `union` (el mismo de la pila), por lo que
  debemos especificar qué parte usar para almacenar el lexema.
- Los lexemas del token `NUMBER` está conformado por dígitos y el punto decimal.
- Usamos `mem[26]` como _tabla de símbolos_.

## Diferencias entre versiones de HOC

- HOC1 es una calculadora básica
- HOC2
  - ahora tiene variables (26)
  - arreglo como memoria [_tabla de símbolos_] (26 posiciones)
- HOC3 calculadora científica
  - lista simplemente ligada como tabla de símbolos
- HOC4 es un puente entre 3 y 5., utiliza una Maquina Virtual de pila
- HOC5 if, ciclos
- HOC6 funciones y procedimientos

## HOC3

```yacc
struct symbol {
  char *name;
  int type;
  union {
    double val;
    double (*ptr)(double);
  } u;
  struct symbol *next;
};
typedef struct symbol Symbol;
static Symbol *symlist = 0;

Symbol *lookup(char *s)
{
  Symbol *sp;
  for (sp = symlist; sp ; sp = sp->next)
  {
    if (!strcmp(sp->name, s))
    {
      return sp;
    }
  }
  return (Symbol *)NULL;
}

Symbol *install(char *s, int t, double d)
{
  Symbol *sp = (Symbol*)malloc(sizeof(Symbol));
  sp->name = (char*)malloc(strlen(s) + 1);
  strcpy(sp->name, s);
  sp->type = t;
  sp->u.val = d;
  sp->next = symlist;
  symlist = sp;
  return sp;
}

%{
  #include "hoc.h"
  ...
}
%union {
  double val;
  Symbol *sym;
}

%token <val> NUMBER
%token <sym> VAR BLTIN INDEF
%type <val> exp asgn

list:
  ...
  | list asgn '\n'
  ...
asgn: VAR '=' exp {
    $$ = $1 -> u.val = $3;
    $1 -> type = VAR;
  }
  ;
exp: NUMBER { $$ = $1; }
  | VAR {
      if ($1 -> type == INDEF)
      {
        puts("varaible no definida");
        $$ = $1 -> u.val;
      }
    }
  | asgn
  | BLTIN'('exp')' {
      $$ = ($1 -> u.ptr)($3);
    }
  | exp'^'exp {
      $$ = pow($1, $3);
    }
  ...

%%

void main()
{
  init();
  yyparse();
}

int yylex()
{
  ...
  if (isalpha(c))
  {
    Symbol *s;
    char sbuf[512], *p = sbuf;
    do
    {
      *p++ = c;
    } while (isalnum(c = getchar()));
    ungetc(c, stdin);
    *p = '\0';
    if ((s = lookup(sbuf)) == 0)
    {
      s = install(sbuf, INDEF, 0.0);
    }
    yylval.sym = s;
    if (s->type == INDEF)
    {
      return VAR;
    }
    return s->type;
  }
  return c;
}

#include "hoc.h"

static struct {
  char *name;
  double eval;
} consts[] = {
  "PI", 3.14159265358979323846,
  "E", 2.7182818284590452354,
  "GAMMA", 0.57721566490153286060,
  ...,
  0, 0
};

static struct {
  char *name;
  double (*func)(double);
} builtins[] = {
  "sin", sin,
  "cos", cos,
  "atan", atan,
  ...,
  0, 0
};

void init()
{
  int i;
  Symbol *s;
  for (i = 0; consts[i].name; i++)
  {
    install(consts[i].name, VAR, consts[i].eval);
  }

  for (i = 0; builtins[i].name; i++)
  {
    s = install(builtins[i].name, BLTIN, 0.0);
    s->u.ptr = builtins[i].func;
  }
}
```

- La tabla de símbolos es una lista simplemente ligada.

## HOC4

```
typedef void (*Inst)(void);

#define STOP (Inst)0;
...
%union {
  Symbol *sym;
  Inst *inst;
}
%token <sym> NUMBER VAR BLTIN INDEF
...
list:
  | list asgn{ code2(pop1, STOP); return 1; }
  ...
asgn: VAR'='exp{ code3(varpush, (Inst)$1, assign); }
  ;
exp: NUMBER {code2(constpush, (Inst)$1);}
  | VAR {code3(varpush,(Inst)$1,eval);}
  | asgn
  ...
  | exp'+'exp{code(add);}
  | exp'*'exp{code(mul);}
  ...

int yylex()
{
  ...
  if(c=='.' || isdigit(c))
  {
    double d;
    ungetc(c, stdin);
    scanf("%lf", &d);
    yylval.sym = install("",NUMBER,d);
    return NUMBER;
  }
  if (isalpha(c))
  {
    Symbol *s;
    char sbuf[100],
        *p = sbuf;
    do
    {
      *p++ = c;
    } while ((c = getchar()) != EOF && isalnum(c));

    *p = '\0';

    if ((s = lookup(sbuf)) == NULL)
    {
      s = install(sbuf, INDEF)
      }
    }
}
```

## HOC 6

- En Pascal hay funciones y procedimientos.
- En lenguaje C solo hay funciones.
- El marco de función en C tiene tres cosas: 
  - Parámetros
  - Dirección de retorno
  - Variables locales
- Tenemos 3 pilas:
  - Pila de YACC
  - Pila de la Máquina Virtual de Pila
  - Pila de llamadas
- Los nombres de funciones y variables se almacenan en la tabla de símbolos.

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
: Acciones gramaticales

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

Union vs. Struct en C
: Las estructuras reservan en memoria el espacio para todos sus miembros y
en la unión solo se reserva el espacio más grande.

Para HOC3 - Nodo
: Entradas en la tabla de símbolos

Expresión Booleana
: Una expresión que, al ser evaluada, devuelve un verdad o falso.

Procedimiento
: Conjunto de instrucciones asociadas a un nombre.

Variable
: Una localidad de memoria que tiene asociado un nombre.
: Nombre que se le da a una localidad de memoria.

Mecanismo de llamada a función
: Mecanismo que se utiliza para pasar parámetros a una función.

Funcionamiento del mecanismo de llamada a función
: Se pasa el valor de los parámetros a la función.

Dirección de retorno
: La dirección de memoria despues de la invocación (llamada) de una función.