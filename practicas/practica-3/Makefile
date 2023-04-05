# Makefile para la práctica 1
# En este archivo se definen las tareas de compilación requeridas para poder 
# llevar a cabo la generación del código objeto y finalmente el ejecutable
# con la calculadora.
# Humberto Alejandro Ortega Alcocer

# Son los archivos necesarios de YACC
Gram=y.tab.c y.tab.h

# Tarea de compilación general
all: $(Gram) lex.yy.c
	@gcc -o complejos.out y.tab.c lex.yy.c complejo_calc.c
	@echo ¡Compilado!

# Compilar de YACC a C
$(Gram): complejo_calc.y
	@yacc -d complejo_calc.y
	@echo ¡Compilado con YACC!

# Compilar de LEX a C
lex.yy.c: complejo_calc.l
	@flex complejo_calc.l
	@echo ¡Compilado con Lex!

# Operación de limpieza
clean:
	@rm -f lex.yy.c *.tab.* *.out
	@echo ¡Archivos eliminados!

