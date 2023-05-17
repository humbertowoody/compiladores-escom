// Librerías estándar.
#include <stdio.h>  // Entrada y salida estándar.
#include <stdlib.h> // Funciones de la biblioteca estándar.
#include <string.h> // Funciones para manejar cadenas de caracteres.
#include <math.h>   // Funciones matemáticas.

// Librerías propias.
#include "vectores.h" // Funciones para manejar vectores y tabla de símbolos.
#include "y.tab.h"    // Definiciones de los tokens.

// Tabla de símbolos, lista ligada.
static Symbol *symlist = 0;

/**
 * @brief Función que crea un vector de tamaño n y con el valor de dato en la primera posición.
 *
 * @param n El número de elementos del vector.
 * @param dato El valor del primer elemento del vector.
 * @return Vector* Un apuntador al vector creado.
 */
Vector *creaVector(int n, double dato)
{
  // Variables locales.
  Vector *vec; // Apuntador al vector creado.
  int i;       // Contador.

  // Reservamos memoria para el vector.
  vec = (Vector *)malloc(sizeof(Vector));

  // Inicializamos los valores del vector.
  vec->n = n;
  vec->vec = (double *)malloc(sizeof(double) * n); // Aquí asignamos memoria para el arreglo de n doubles.
  vec->vec[0] = dato;

  // Regresamos el vector creado.
  return vec;
}

/**
 * @brief Función que imprime un vector.
 *
 * @param v Un apuntador al vector a imprimir.
 */
void imprimeVector(Vector *v)
{
  // Variables locales.
  int i; // Iterador.

  // Imprimimos el vector.
  for (i = 0; i < v->n; i++)
  {
    printf("%.2f ", v->vec[i]);
  }

  // Salto de línea.
  printf("\n");
}

/**
 * @brief Función que copia un vector.
 *
 * @param v El vector original.
 * @return Vector* El nuevo vector copia del original.
 */
Vector *copiaVector(Vector *v)
{
  // Variables locales.
  int i;        // Iterador.
  Vector *copy; // Vector destino.

  // Creamos un nuevo vector de las mismas dimensiones.
  copy = creaVector(v->n, 0);

  // Copiamos los elementos del vector original en el destino.
  for (i = 0; i < v->n; i++)
  {
    copy->vec[i] = v->vec[i];
  }

  // Regresamos el apuntador a la copia.
  return copy;
}

/**
 * @brief Realiza la suma de dos vectores.
 *
 * @param a El primer vector.
 * @param b El segundo vector.
 * @return Vector* El vector resultante.
 */
Vector *sumaVector(Vector *a, Vector *b)
{
  // Variables locales.
  Vector *c; // Vector para almacenar el resultado.
  int i;     // Iterador.

  // Creamos el vector para almacenar el resultado.
  c = creaVector(a->n, 0);
  // Iteramos sobre los elementos de los vectores.
  for (i = 0; i < a->n; i++)
  {
    // Realizamos la suma.
    c->vec[i] = a->vec[i] + b->vec[i];
  }

  // Regresamos el vector con el resultado.
  return c;
}

/**
 * @brief Resta de dos vectores.
 *
 * @param a El primer vector.
 * @param b El segundo vector.
 * @return Vector* El vector resultante de la operación.
 */
Vector *restaVector(Vector *a, Vector *b)
{
  // Variables locales.
  Vector *c; // Vector para almacenar el resultado.
  int i;     // Iterador

  // Creamos el vector para almacenar el resultado.
  c = creaVector(a->n, 0);

  // Realizamos la operación.
  for (i = 0; i < a->n; i++)
  {
    c->vec[i] = a->vec[i] - b->vec[i];
  }

  // Regresamos el vector con el resultado.
  return c;
}

/**
 * @brief Une dos vectores secuencialmente.
 *
 * @param a El primer vector.
 * @param b El segundo vector.
 * @return Vector* El vector resultante de los elementos de a seguidos de b.
 */
Vector *unirVectores(Vector *a, Vector *b)
{
  // Variables locales.
  Vector *c; // El vector para el resultado.
  int i,     // Iterador.
      len;   // La longitud de los dos vectores.

  // Calculamos la longitud de los dos vectores.
  len = a->n + b->n;

  // Creamos un vector con la longitud de ambos vectores unidos.
  c = creaVector(len, 0);

  // Copiamos los elementos del primer vector en el vector resultante.
  for (i = 0; i < a->n; i++)
  {
    c->vec[i] = a->vec[i];
  }

  // Copiamos los elementos del segundo vector en el vector resultante.
  for (; i < len; i++)
  {
    c->vec[i] = b->vec[len - i];
  }

  // Regresamos el vector resultante.
  return c;
}

/**
 * @brief Función que calcula el producto punto de dos vectores.
 *
 * @param a Primer vector.
 * @param b Segundo vector.
 * @return double El resultado, escalar, de la operación.
 */
double productoPunto(Vector *a, Vector *b)
{
  // Variables locales.
  int i,          // Iterador.
      len;        // Longitud  del vector más pequeño.
  double res = 0; // Variable para almacenar el resultado.

  // Calculamos la longitud del vector más pequeño.
  len = a->n > b->n ? b->n : a->n;

  // Realizamos la operación.
  for (i = 0; i < len; i++)
  {
    res += a->vec[i] * b->vec[i];
  }

  // Retornamos el resultado (escalar).
  return res;
}

/**
 * @brief Función que calcula el producto cruz de dos vectores.
 *
 * @param a Primer vector.
 * @param b Segundo vector.
 * @return Vector* El resultado de la operación.
 */
Vector *productoCruz(Vector *a, Vector *b)
{
  // Variables locales.
  Vector *c; // Vector con el resultado de la operación.

  // Creamos el vector con el tamaño resultante.
  c = creaVector(3, 0);

  // Realizamos la operación del producto cruz.
  c->vec[0] = a->vec[1] * b->vec[2] - b->vec[1] * a->vec[2];
  c->vec[1] = a->vec[2] * b->vec[0] - b->vec[2] * a->vec[0];
  c->vec[2] = a->vec[0] * b->vec[1] - b->vec[0] * a->vec[1];

  // Regresamos el vector resultante.
  return c;
}

/**
 * @brief Función que calcula el producto escalar de un vector y un escalar.
 *
 * @param a Un escalar.
 * @param b Un vector.
 * @return Vector* El restulado de la operación.
 */
Vector *productoEscalar(double a, Vector *b)
{
  // Variables locales.
  Vector *c; // El vector para almacenar el resultado.
  int i;     // Iterador.

  // Creamos el vector con la longitud requerida.
  c = creaVector(b->n, 0);

  // Iteramos sobre el vector y realizamos la operación.
  for (i = 0; i < b->n; i++)
  {
    c->vec[i] = a * b->vec[i];
  }

  // Regresamos el vector resultante.
  return c;
}

/**
 * @brief Función que calcula la magnitud de un vector.
 *
 * @param a El vector del cual se quiere calcular la magnitud.
 * @return double La magnitud del vector.
 */
double magnitud(Vector *a)
{
  // Variables locales.
  double res = 0; // El resultado de la operación.
  int i;          // Iterador.

  // Realizamos la operación.
  for (i = 0; i < a->n; i++)
  {
    res += a->vec[i] * a->vec[i];
  }

  // Regresamos el resultado.
  return sqrt(res);
}

/**
 * @brief Función que busca un símbolo en la tabla de símbolos.
 *
 * @param s El nombre del símbolo a buscar.
 * @return Symbol* El símbolo encontrado (o NULL, 0).
 */
Symbol *lookup(char *s)
{
  // Variables locales.
  Symbol *sp; // Iterador para la tabla de símbolos.

  // Iteramos sobre la tabla de símbolos.
  for (sp = symlist; sp != (Symbol *)0; sp = sp->next)
  {
    // Validamos si el nombre del símbolo es igual al que buscamos.
    if (strcmp(sp->name, s) == 0)
    {
      // Si lo encontramos, lo regresamos.
      return sp;
    }
  }

  // Si no lo encontramos, regresamos 0 (NULL).
  return 0;
}

/**
 * @brief Función que instala un símbolo en la tabla de símbolos.
 *
 * @param s El nombre del símbolo.
 * @param t El tipo del símbolo.
 * @param d El Vector del símbolo.
 * @return Symbol* Un apuntador al símbolo instalado en la tabla de símbolos.
 */
Symbol *install(char *s, int t, Vector *d)
{
  // Variables locales.
  Symbol *sp; // El símbolo a instalar.

  // Asignamos memoria para el símbolo.
  sp = (Symbol *)malloc(sizeof(Symbol));

  // Asignamos la memoria para el nombre del símbolo.
  sp->name = (char *)malloc(strlen(s) + 1);

  // Copiamos el nombre en el nombre del símbolo.
  strcpy(sp->name, s);

  // Asignamos los valores al símbolo.
  sp->type = t;
  sp->u.val = d;
  sp->next = symlist; // Lo colocamos al inicio de la lista simplemente ligada.
  symlist = sp;

  // Regresamos el símbolo.
  return sp;
}
