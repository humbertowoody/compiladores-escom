
#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct vector
{
	char name;
	int n;
	double *vec;
};
typedef struct vector Vector;

Vector *creaVector(int n);

void imprimeVector(Vector *a);

int vectoresIguales(Vector *a, Vector *b);

Vector *sumaVector(Vector *a, Vector *b);

Vector *restaVector(Vector *a, Vector *b);

Vector *cruzVector(Vector *a, Vector *b);

double puntoVector(Vector *a, Vector *b);

Vector *multiEscalarVector(double num, Vector *a);

double magnitudVector(Vector *a);

#endif