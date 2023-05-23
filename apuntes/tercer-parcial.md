# Tercer Parcial

## Análisis sintáctico predictivo no recursivo

Entrada

## $PRIMERO(\alpha)$

Si alpha es una cadena de símbolos gramaticales $PRIM(\alpha)$ es el conjunto
de tokens que inicializa cadenas derivadas de $\alpha$.

## $SIGUIENTE(A)$

Se define $SIGUIENTE(A)$ para el no terminal $A$, como el conjunto de tokens que
pueden aparecer inmediatamente a la derecha de $A$ en alguna forma de frase, es
decir, el conjunto de tokens a tal que haya una derivación de la forma $S \Rightarrow xAaB$
para algún $x$ y $B$. Si $A$ puede ser el situado más a la derecha en una forma
de frase entonces $\$$ está en $SIG(A)$.

## Reglas

Para calcular $PRIM(X) \forall$ los símbolos gramáticales $X$. Aplíquense las
reglas siguientes hasta que no se puedan añadir más tokens o t a ningún conjunto $PRIM$.

1. Si $X$ es un token (terminal), entonces $PRIM(X)$ es $\{X\}$.
2. Si $X \rightarrow \epsilon$ es una producción entonces añádase $\epsilon$ a $PRIM(X)$.
3. Si $X$ es no terminal y $X \rightarrow Y_1 Y_2 ... Y_k$ es una producción entonces
   póngase $a$ en $PRIM(X)$.

Si para alguna $i$, $a$ está en $PRIM(Y_i)$ y $\epsilon$ está en $\forall$ los
$PRIM(Y_1), ... , PRIM(Y_{i-1})$; es decir $Y_1 ... Y_{i-1} \rightarrow \epsilon$.
Si $\epsilon$ está en $PRIM(Y_j) \forall j=1,2,...,k$, entonces añádase $\epsilon$
a $PRIM(X)$. Si $Y_1$ no deriva $\epsilon$, entonces no se añade nada más a $PRIM(X)$,
pero si $Y_1 \rightarrow \epsilon$ entonces se le añade $PRIM(Y_2)$ y así sucesivamente.

Se puede calcular $PRIMERO \forall$ cadena $X1,X_2,...,X_n$ (esto es un $\alpha$)
de la siguiente forma añádanse a $PRIM(X_1 X_2 ... X_n)$ $\forall$ los símbolos
distintos de $\epsilon$ de $PRIM(X_1)$, si $\epsilon$ está en $PRIM(X_1)$, añádanse
también símbolos distintos de $\epsilon$ de $PRIM(X_2)$. Si $\epsilon$ está en $PRIM(X_2)$
como en $PRIM(X_2)$, añádanse también los símbolos distintos de $\epsilon$ de
$PRIM(X_3)$ y así sucesivamente. Por último, añádase $\epsilon$ a $PRIM(X_1 X_2 .. X_n)$
si $\forall i PRIM(X_i)$ contiene $\epsilon$.

Para calcular $SIG(A) \forall$ los no terminales $A$. Aplíquese las reglas
siguientes hasta que no puedan añadirse nada más a ningún conjunto $SIG$.

1. Póngase $\$$ en $SIGUIENTE(\$)$.
2. Si hay una producción $A \rightarrow \alpha B \beta$ entonces \forall$ la que
   esté en $PRIM(\beta)$ excepto $\epsilon$ se pone en $SIG(B)$.
3. Si hay una producción $A \rightarrow \alpha B$ o una producción $A \rightarrow \alpha B \beta$
   dónde $PRIM(\beta)$ contenga $\epsilon$ (es decir $\beta \rightarrow \epsilon$) entonces
   $\forall$ lo que esté en $SIG(A)$ se pone en $SIG(B)$.

## Glosario

$
: es el fin de la cadena.
