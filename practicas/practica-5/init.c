
#include "symbol.h"
#include "y.tab.h"

static struct
{ /* Constantes */
	char *name;
	double cval;
} consts[] = {
		"PI", 3.14159265358979323846,
		"E", 2.71828182845904523536,
		"GAMMA", 0.57721566490153286060, /* Euler */
		"DEG", 57.29577951308232087680,	 /* grado/radian */
		"PHI", 1.6180339887498948 * 820, /* proporcion dorada */
		0, 0};

static struct
{
	char *name; /* Palabras clave */
	int kval;
} keywords[] = {
		"if",
		IF,
		"else",
		ELSE,
		"while",
		WHILE,
		"print",
		PRINT,
		0,
		0,
};

initConstants() /* instalar constantes y predefinidos en la tabla */
{
	int i;
	Symbol *s;
	for (i = 0; keywords[i].name; i++)
		install(keywords[i].name, keywords[i].kval, 0.0);
	for (i = 0; consts[i].name; i++)
		s = install(consts[i].name, VARESCALAR, consts[i].cval);
	s->u.varVector = NULL;
}
