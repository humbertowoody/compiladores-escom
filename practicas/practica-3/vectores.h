#ifndef VECTORES_H_INCLUIDO
#define VECTORES_H_INCLUIDO

// Estructura del vector.
struct vector
{
  char name;   // Nombre del vector.
  int n;       // Número de elementos del vector.
  double *vec; // Arreglo con los contenidos del vector.
};

// Definición del tipo Vector para englobar la estructura.
typedef struct vector Vector;

// Estructura para una entrada en la tabla de símbolos.
typedef struct Symbol
{
  char *name; // Nombre del símbolo.
  short type; // Tipo del símbolo: VAR, BLTIN, UNDEF.
  union
  {
    Vector *val;       // Si es VAR para almacenar el vector asociado a la variable.
    double (*ptr)();   // Si es BLTIN para almacenar la función asociada.
  } u;                 // Unión para almacenar el valor o la función.
  struct Symbol *next; // Apuntador al siguiente elemento en la tabla de símbolos.
} Symbol;

// Prototipos de funciones de la tabla de símbolos.
Symbol *install(char *, int, Vector *);
Symbol *lookup(char *s);

// Prototipos de funciones para operaciones con Vectores.
Vector *creaVector(int n, double dato);
void imprimeVector(Vector *a);
Vector *unirVectores(Vector *a, Vector *b);
Vector *copiaVector(Vector *a);
Vector *sumaVector(Vector *a, Vector *b);
Vector *restaVector(Vector *a, Vector *b);
double productoPunto(Vector *a, Vector *b);
Vector *productoCruz(Vector *a, Vector *b);
Vector *productoEscalar(double a, Vector *b);
double magnitud(Vector *a);

#endif // VECTORES_H_INCLUIDO