Gram=y.tab.c y.tab.h

all: $(Gram) 
	@gcc-13 -o vectores.out y.tab.c vectores.c -lm 
	@echo ¡Programa compilado con GCC!

$(Gram): y.tab.c
	@yacc -y -d vectores.y
	@echo ¡Gramática compilada con YACC!

clean:
	@rm -f *.out  *.tab.* *.o
	@echo ¡Archivos eliminados!
