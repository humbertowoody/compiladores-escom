# Árbol de Análisis Sintáctico (AAS)

Un A.A.S. es un árbol con las siguientes 4 propiedades:

1. Raíz etiquetada con el símbolo inicial de la gramática.
2. Las hojas deben estar etiquetadas con un token o con _épsilon_.
3. Cada nodo interior está etiquetado con un _no terminal_.
4. Si $A$ es el _no terminal_ que etiqueta a algún nodo interior y $X_1, X_2, ..., X_n$
   son las etiquetas de lo hijos de ese nodo de izquierda a derecha entonces
   $A \rightarrow X_1, X_2, ..., X_n$ es una producción. Si $A \rightarrow épsilon$

## Ambigüedad

Una gramática es ambigua si existe una cadena que perteneceal lenguaje generado
por la gramática para la cual se pueden dibujar dos AAS diferentes.

## Asociatividad de Operadores

Los operadores en una GLC retienen sus reglas de asociatividad, con estas reglas 
es que podemos reducir cada una de las expresiones que se nos plantean para poder 
evaluarla de forma correcta.

## Notación Postfija

1. Si $E$ es una variable o una constante entonces la notación postfija de $E$ es 
también $E$.
2. Si $E$ es una expresión de la forma $E_1$ operador $E_2$ entonces la notación 
postfija es $E_1'$ $E_2'$ operador dónde $E_1'$ y $E_2'$ son las  instrucciones 
postfijas de $E_1$ y $E_2$, respectivamente.
3. Si $E$ es una expresión de la forma $(E_1)$ entonces la notación postfija de 
$E_1$ es también la notación postfija de $E$.


## Definición Definida por la Sintaxis (DDS)

Una DDS asocia a cada símbolo gramatical un conjunto de atributos y a cada 
producción un conjunto de reglas semánticas para calcular los valores de los 
atributos asociados con los símbolos gramáticales que aparecen en esa producción.

- DDS = Gramática Libre de Contexto + Reglas Sintácticas
- Sirve para traducir.

## A.A.S. con anotaciones (Árbol Decorado)

Es el A.A.S. dónde en cada nodo se muestran los valores de los atributos. 

Si un nodo $n$ está etiquetado con el símbolo gramatical $X$ entonces se 
escribe $X.a$ para indicar el valor del atributo $a$ de $X$ en el nodo $n$.

## Atributo Sintetizado 

Es aquél atributo cuyo valor en un nodo del A.A.S. se determina a partir de los 
valores de los atributos de los hijos de ese nodo.

## Recorrido en profundidad

```txt
void visita(Nodo n) {
    for (cada nodo m hijo de n de izquierda a derecha) {
      visita(m);
    }
    evalua las reglas semanticas en el nodo n;
}
```

En árboles tenemos tres recorridos base:

- Postorden: izq, der, raíz.
- Inorden: izq, raíz, der.
- Preorden: raíz, izq, der.

Algunas carácterísticas de los árboles:

- Un **árbol binario** puede tener a lo más dos hijos.
- Un **árbol multicamino** puede tener más de dos hijos.

## Esquema de Traducción

Es una GLC en la que se encuentran intercalados en los lados derechos de las
producciones fragmentos de programa llamados **acciones semánticas**.

- Esquema de Traducción = GLC + acciones semánticas.
- Sirve para traducir.

Queremos obtener la traducción usando un esquema de traducción, para esto los
pasos son:

1. **Dibujar** el árbol de análisis sintáctico a partir de la expresión. Los
hijos putativos del árbol son las acciones semánticas.
2. Colgar del árbol de análisis sintáctico las acciones semánticas.
3. Recorrer en profundidad el árbol.
   - Si durante el recorrido encontramos un nodo que es una _acción semántica_,
   ejecutamos la acción semántica.

## Análisis Sintáctico Descendente

El árbol se construye a partir de la raíz y se va avanzando hacia las hojas.

### Análisis Sintáctico Descendente Recursivo

- Se procesa la entrada usando un conjunto de procedimientos que pueden ser
recursivos para analizar la entrada.

### Análisis Sintáctico Descendente Predictivo Recursivo

```txt
tipo -> simple
     | ^ id
     | array[simple] of tipo
simple -> INTEGER
       | CHAR
       | NUM PTOPTO NUM
```

```c
int preana; // Variable global, almacena el token actual.

// En inglés esta función se llama "match"
void parea(Complex t)
{
  if (preana == t)
  {
    // Siguiente componente léxico.
    preana = sigcomplex(); // Siguiente token.
  }
  else
  {
    error();
  }
}

void tipo()
{
  if (preana == INTEGER ||
      preana == CHAR ||
      preana == NUM )
  {
    simple();
  }
  else if (preana == '^')
  {
    parea('^');
    parea(ID);
  }
  else if (preana == ARRAY)
  {
    parea(ARRAY);
    parea('[');
    simple();
    parea(']');
    parea(OF);
    tipo();
  }
  else
  {
    error();
  }
}

void simple()
{
  if (preana == INTEGER)
  {
    parea(INTEGER);
  }
  else if (preana == CHAR)
  {
    parea(CHAR);
  }
  else if (preana == NUM)
  {
    parea(NUM);
    parea(PTOPTO);
    parea(NUM);
  }
  else
  {
    error();
  }
}
```

## Glosario de Términos

- _Acción Semántica_: Fragmento del programa que se encuentra intercalada en
el lado derecho de una producción.
- _Hijo Putativo_: es una persona que es bien recibida en casa de alguien más.
- _Circunflejo_: `^`
