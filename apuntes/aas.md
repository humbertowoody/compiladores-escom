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

## A.A.S. con anotaciones (Árbol Decorado)

Es el A.A.S. dónde en cada nodo se muestran los valores de los atributos. 

Si un nodo $n$ está etiquetado con el símbolo gramatical $X$ entonces se 
escribe $X.a$ para indicar el valor del atributo $a$ de $X$ en el nodo $n$.

## Atributo Sintetizado 

Es aquél atributo cuyo valor en un nodo del A.A.S. se determina a partir de los 
valores de los atributos de los hijos de ese nodo.

