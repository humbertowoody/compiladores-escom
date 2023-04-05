typedef struct Symbol { /* entrada de la tabla de símbolos */
	char   *name;
	short   type;   /* VAR, BLTIN, INDEF */
	union {
		double val;	       /* si es VAR */
		double  (*ptr)(double);      /* sí es BLTIN */
	} u;
	struct Symbol   *next;  /* para ligarse al sig. */ 
} Symbol;

Symbol *install(char *s,int t, double d), *lookup(char *s);


double  ptr; //double
double  *ptr;//apun a doble
double  ptr(double); //func recibe double y ret. double
double  *ptr(double);//func recibe double y ret apun a double


