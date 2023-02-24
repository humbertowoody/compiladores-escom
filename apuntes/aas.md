# Árbol de Análisis Sintáctico (AAS)

Un AAS es un árbol con las siguientes 4 propiedades:

1. Raíz etiquetada con el símbolo inicial de la gramática.
2. Las hojas deben estar etiquetadas con un token o con _épsilon_.
3. Cada nodo interior está etiquetado con un _no terminal_.
4. Si `A` es el _no terminal_ que etiqueta a algún nodo interior y `X_1, X_2, ..., X_n`
   son las etiquetas de lo hijos de ese nodo de izquierda a derecha entonces
   `A -> X_1, X_2, ..., X_n` es una producción. Si `A -> épsilon`
