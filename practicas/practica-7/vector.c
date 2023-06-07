
#include "vector.h"

Vector *creaVector(int n)
{
   Vector *vec;
   int i;
   vec = (Vector *)malloc(sizeof(Vector));
   vec->n = n;
   vec->vec = (double *)malloc(sizeof(double) * n);
   return vec;
}

void imprimeVector(Vector *v)
{
   printf("\033[0;36m");
   if (v != NULL)
   {
      int i;
      printf("[ ");
      for (i = 0; i < v->n; i++)
         printf("%f ", v->vec[i]);
      printf("]\n");
   }
   printf("\033[0;0m");
}

int vectoresIguales(Vector *a, Vector *b)
{
   int iguales = a->n == b->n;
   if (!iguales)
      printf("\x1b[31m Error: Vectores de dimension diferente\n\x1b[0m");
   return iguales;
}

Vector *sumaVector(Vector *a, Vector *b)
{
   Vector *c = NULL;
   if (vectoresIguales(a, b))
   {
      int i;
      c = creaVector(a->n);
      for (i = 0; i < a->n; i++)
         c->vec[i] = a->vec[i] + b->vec[i];
   }
   return c;
}

Vector *restaVector(Vector *a, Vector *b)
{
   Vector *c = NULL;
   if (vectoresIguales(a, b))
   {
      int i;
      c = creaVector(a->n);
      for (i = 0; i < a->n; i++)
         c->vec[i] = a->vec[i] - b->vec[i];
   }
   return c;
}

Vector *cruzVector(Vector *a, Vector *b)
{
   Vector *c = NULL;
   if (a->n != 3 || b->n != 3)
   {
      printf("\x1b[31m Error: Producto cruz solo posible con R3\n\x1b[0m");
   }
   else
   {
      int i;
      c = creaVector(3);
      c->vec[0] = (a->vec[1] * b->vec[2]) - (a->vec[2] * b->vec[1]);
      c->vec[1] = (a->vec[2] * b->vec[0]) - (a->vec[0] * b->vec[2]);
      c->vec[2] = (a->vec[0] * b->vec[1]) - (a->vec[1] * b->vec[0]);
   }
   return c;
}

double puntoVector(Vector *a, Vector *b)
{
   int i;
   double productoPunto = 0;
   if (vectoresIguales(a, b))
   {
      for (i = 0; i < a->n; i++)
      {
         productoPunto += a->vec[i] * b->vec[i];
      }
   }
   return productoPunto;
}

Vector *multiEscalarVector(double num, Vector *a)
{
   int i;
   Vector *c = NULL;
   c = creaVector(a->n);
   for (i = 0; i < a->n; i++)
      c->vec[i] = a->vec[i] * num;
   return c;
}

double magnitudVector(Vector *a)
{
   int i;
   double magnitud = 0;
   for (i = 0; i < a->n; i++)
   {
      magnitud += pow(a->vec[i], 2);
   }
   return sqrt(magnitud);
}