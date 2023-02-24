# ¿Qué es un compilador?

Es un programa que traduce un programa escrito en lenguaje fuente a un programa en
lenguaje objeto y que tenga una equivalencia.

Un compilador consta de dos partes:

- **Análisis**: En esta etapa se _separa_ el programa en sus partes más relevantes.
- **Síntesis**: En esta etapa se _ensambla_ (mala palabra) el programa final.

## Etapas de un compilador.

1. **Análisis Léxico**: Separa la cadena de entrada en tokens.
2. **Análisis Sintáctico**:
   1. Verifica que los _tokens_ vengan en el orden correcto.
   2. Construye el Árbol Abstracto de Sintaxis (A.A.S.).
   3. Defina si la cadena pertenece al lenguaje generado por la gramática (dicha
      cadena está conformada por _tokens_).
3. **Análisis Semántico**:
   1. Revisa que los operandos de una operación estén correctos.
   2. Conversión de tipos.
4. **Generación de Código Intermedio**:
   - Equivalente al programa que le dimos como entrada al compilador.
   - _Equivalente_: que haga lo mismo.
   - Es fácil de generar.
   - Tipos de código intermedio:
     1. _Código de 3 direcciones_:
        - Máximo de 3 operandos y máximo 2 operadores por línea.
     2. _Postfijo_.
     3. _A.A.S._
5. **Optimización de Código**:
6. **Generación de Código**:

## Glosario de Términos.

- **Componente Léxico**: Conjunto de caracteres con significado colectivo.
- **Token**: Se definen por el lenguaje en cuestión.
- **Cardinalidad**: Es el número de elementos en un conjunto.

## Fotografías del Pizarrón.
