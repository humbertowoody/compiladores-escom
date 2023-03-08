# Gramática

Cuadrupla de 4 conjuntos:

1. **T**: Conjunto de Terminales
2. **N**: Conjunto de No Terminales
3. **S**: Símbolo Inicial
4. **P**: Conjunto de producciones.

$G(T,N,S,P)$

## Gramática Libre de Contexto

1. Conjunto de Tokens
2. Conjunto de No Terminales
3. Conjunto de producciones
   - Cada producción consta de un no terminal llamada _lado izquierdo de la producción_ y una flecha y una secuencia de Tokens y no terminales o ambos llamado _lado derecho de la producción_.
4. La denominación de los no terminales como _símbolo inicial_.

### Ejemplo de Gramática Libre de Contexto (GLC)

Definición de gramática:

```
lista -> lista + dígito
lista -> lista - dígito
lista -> dígito
dígito -> 0|1|2|3|4|5|6|7|8|9
```

``` 
T={0,1,2,3,4,5,6,7,8,9}
N={lista, dígito}
S={lista}
```

```
5+2-9
lista -> lista - dígito
-> lista + dígito - dígito
-> dígito + dígito - dígito
-> 5 + dígito - dígito
-> 5 + 2 - dígito
-> 5 + 2 - 9
```

## Glosario de Términos

Producción
: Es algo que tiene lado izquierdo (no terminal) y lado derecho 
(cadena de símbolos gramaticales), son las reglas de sustitución.

Símbolo Gramatical
: es un token o un no terminal.

Terminal
: es lo mismo que _token_.

Token  
: Componente.
: Terminal.
