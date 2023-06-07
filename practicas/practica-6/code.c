#include "symbol.h" 
#include "y.tab.h"
#define NSTACK  256
static  Datum  stack[NSTACK];  /* la pila */
static  Datum   *stackp;       /* siguiente lugar libre en la pila */
#define NPROG   20000
Inst    prog[NPROG];    /* la RAM de la maquina VP*/
Inst    *progp;         /* siguiente lugar libre para la generacion de codigo */
Inst    *pc;	/* contador de programa durante la ejecucion */
int     dimVector; /* contador que ayuda a crear los vectores */

void initcode( )      /* inicializacion para la generacion de codigo */ {
   stackp = stack;
   progp = prog;
   dimVector=0;
}

void push(Datum d){	/*  meter d en la pila  */
   if (stackp >= &stack[NSTACK])
      printf("\x1b[31m%s\n\x1b[0m","stack overflow"); 
   *stackp++ = d;
}

Datum pop( ){     /* sacar y retornar de la pila el elemento del tope */
   if (stackp <= stack)
      printf("\x1b[31m%s\n\x1b[0m","stack underflow"); 
   return  *--stackp;
}

void pop1( ){     /* sacar y retornar de la pila el elemento del tope */
   if (stackp <= stack)
      printf("\x1b[31m%s\n\x1b[0m","stack underflow"); 
   --stackp;
}

void escalarpush(){ /* meter un escalar a la pila  */
   Datum d;
   d.num  =  ((Symbol  *)*pc++)->u.varEscalar;
   push(d);
}

void numpush( ){	/* meter un numero para crear un vector a la pila  */
   escalarpush();
   dimVector++;
}

void vectorpush( ){
   Vector *vec=creaVector(dimVector);
   int i;
   for(i=0;i<dimVector;i++){
       Datum t = pop();
       vec->vec[i] = t.num;
   }
   Datum d;
   d.vect = vec;
   push(d);
   dimVector=0;
}

void varpush(){	/* meter una variable a la pila   */
   Datum d;
   d.sym  =  (Symbol   *)(*pc++);
   push(d);
}

void evalEscalar( ){	/*  evaluar una variable escalar en la pila   */
   Datum  d;
   d = pop();
   if   (d.sym->type   ==   INDEF)
      printf("\x1b[31mVariable no definida: %s\x1b[0m\n",d.sym->name);
   d.num = d.sym->u.varEscalar;
   push(d);
}

void evalVector( ){ 	/*  evaluar una variable vectorial en la pila   */
   Datum  d;
   d = pop();
   if   (d.sym->type   ==   INDEF || d.sym->type   ==   VAR){
      printf("\x1b[31mVariable no definida: %s\x1b[0m\n",d.sym->name);
      d.vect = NULL;
   }else{
      d.vect = d.sym->u.varVector;
   }
   
   push(d);
}

void assignEscalar( ){ /* asignar el valor escalar superior al siguientevalor */ 
   Datum d1, d2;
   d1 = pop();
   d2 = pop();

   d1.sym->u.varEscalar = d2.num;
   d1.sym->type = VARESCALAR;
   push(d2); 
} 

void assignVector( ){ /* asignar el valor escalar superior al siguientevalor */ 
   Datum d1, d2;
   d1 = pop();
   d2 = pop();

   d1.sym->u.varVector = d2.vect;
   d1.sym->type = VARVECTOR;
   push(d2); 
}

void add( ){
   Datum d1, d2, d3; 
   d2  =  pop(); 
   d1  =  pop(); 
   d3.vect  =  sumaVector(d2.vect,d1.vect);
   push(d3); 
}

void sub(){
   Datum d1, d2, d3; 
   d2  =  pop(); 
   d1  =  pop(); 
   d3.vect  =  restaVector(d2.vect,d1.vect);
   push(d3); 
}

void cross(){
   Datum d1, d2, d3; 
   d2  =  pop(); 
   d1  =  pop(); 
   d3.vect  =  cruzVector(d2.vect,d1.vect);
   push(d3); 
}

void multiEscalarVect(){
   Datum d1, d2, d3; 
   d2  =  pop(); 
   d1  =  pop(); 
   d3.vect  =  multiEscalarVector(d1.num,d2.vect);
   push(d3); 
}

void multiVectEscalar(){
   Datum d1, d2, d3; 
   d2  =  pop(); 
   d1  =  pop(); 
   d3.vect  =  multiEscalarVector(d2.num,d1.vect);
   push(d3); 
}

void magnitude(){
   Datum d1, d2;
   d1 = pop();
   d2.num = magnitudVector(d1.vect);
   push(d2);
}

void point(){
   Datum d1, d2, d3; 
   d2  =  pop(); 
   d1  =  pop(); 
   d3.num  =  puntoVector(d1.vect,d2.vect);
   push(d3); 
}

void printVector( ){  /* sacar el valor vectorial superior de la pila e imprimirlo */ 
 Datum d;
 d = pop();
 imprimeVector(d.vect);
}

void printEscalar( ){  /* sacar el valor escalar superior de la pila e imprimirlo */ 
 Datum d;
 d = pop();
 printf("\033[0;36m%f\n\033[0;0m", d.num);
}

Inst *code(Inst f){ /*   instalar una instruccion u operando   */
   Inst *oprogp = progp;
   if (progp > &prog [ NPROG -1 ])
     printf("\x1b[31m%s\n\x1b[0m","program too big");
   *progp++ = f;
   return oprogp;
}

void execute(Inst *p){	/*   ejecucion con la maquina   */
   for  (pc  =  p;   *pc != STOP; ) 
      (*pc++)();
}

void whilecode() {
   Datum d;
   Inst  *savepc  = pc;	/*  cuerpo de la iteracion  */
   execute(savepc+2);     /*   condicion   */ 
   d  =  pop(); 
   while   (d.num)   {
      execute(*((Inst  **)(savepc)));     /*  cuerpo  */
      execute(savepc+2);
      d  = pop(); 
   }

   pc  =  *((Inst  **)(savepc+1));     /*   siguiente proposicion   */
}

void forcode(){
    Datum d;
    Inst* savepc = pc; /* condicion for */
    execute(savepc + 4); /* Ejecutamos primer stmtfor */
    execute(*((Inst **)(savepc))); /* Ejecutamos condfor */
    d = pop();
    while(d.num){
        execute(* ( (Inst **)(savepc + 2)));   /* Cuerpo del ciclo*/
        execute(* ( (Inst **)(savepc + 1)));   /* Ejecutamos segundo stmtfor */
        execute(*((Inst **)(savepc)));         /* Ejecutamos condfor */
        d = pop();
    }
    pc = *((Inst **)(savepc + 3)); /* Vamos a la proposicion */
}

void ifcode(){
   Datum d;
   Inst  *savepc  = pc;	/* parte then */
   execute(savepc+3);	/*  condicion   */
   d  =  pop(); 
   if (d.num)
      execute(*((Inst   **)(savepc))); 
   else  if   (*((Inst  **)(savepc+1)))   /*  parte else?   */
      execute(*(( Inst  **) (savepc+1)));
   pc  =  *((Inst  **)(savepc+2));	/*  siguiente proposicion   */ 
}

void prexpresc() {      
   printEscalar();
}

void prexprvec() {      
   printVector();
}

//ESCALAR-ESCALAR
void gtEE() {
   Datum d1,  d2;
   d2 = pop();
   d1 = pop();
   d1.num  =   (double)(d1.num  > d2.num);
   push(d1);
}

void ltEE(){
   Datum d1,  d2;
   d2 = pop();
   d1 = pop();
   d1.num  =   (double)(d1.num  < d2.num);
   push(d1);
}

void geEE( ) {
   Datum d1,  d2;
   d2  = pop();
   d1  = pop();
   d1.num   =   (double)(d1.num  >= d2.num);
   push(d1) ;
}

void leEE() {
   Datum d1,  d2;
   d2   =  pop();
   d1   =  pop();
   d1.num  =   (double)(d1.num   <=  d2.num);
   push(d1);
}

void eqEE( ) {
   Datum d1,  d2;
   d2  = pop();
   d1  = pop();
   d1.num  =   (double) (d1.num  ==  d2.num);
   push(d1);
}

void neEE(){
   Datum d1, d2;
   d2 = pop();
   d1 = pop();
   d1.num = (double)(d1.num != d2.num);
   push(d1);
}

void andEE(){
   Datum d1,   d2;
   d2   = pop();
   d1   = pop();
   d1.num = (double)(d1.num   !=   0.0 && d2.num   !=  0.0);
   push(d1);
}

void orEE(){
   Datum d1, d2;
   d2 = pop();
   d1 = pop();
   d1.num = (double)(d1.num != 0.0 || d2.num != 0.0);
   push(d1);
}

void notE( ){
   Datum d;
   d = pop();
   d.num = (double)(d.num == 0.0);
   push(d);
}

//VECTOR-VECTOR
void gtVV() {
   Datum d1,  d2;
   d2 = pop();
   d1 = pop();
   d1.num  =   (double)(magnitudVector(d1.vect)  > magnitudVector(d2.vect));
   push(d1);
}

void ltVV(){
   Datum d1,  d2;
   d2 = pop();
   d1 = pop();
   d1.num  =   (double)(magnitudVector(d1.vect)  < magnitudVector(d2.vect));
   push(d1);
}

void geVV( ) {
   Datum d1,  d2;
   d2  = pop();
   d1  = pop();
   d1.num   =   (double)(magnitudVector(d1.vect)  >= magnitudVector(d2.vect));
   push(d1) ;
}

void leVV() {
   Datum d1,  d2;
   d2   =  pop();
   d1   =  pop();
   d1.num  =   (double)(magnitudVector(d1.vect)   <=  magnitudVector(d2.vect));
   push(d1);
}

void eqVV( ) {
   Datum d1,  d2;
   d2  = pop();
   d1  = pop();
   d1.num  =   (double) (magnitudVector(d1.vect)  ==  magnitudVector(d2.vect));
   push(d1);
}

void neVV(){
   Datum d1, d2;
   d2 = pop();
   d1 = pop();
   d1.num = (double)(magnitudVector(d1.vect) != magnitudVector(d2.vect));
   push(d1);
}

void andVV(){
   Datum d1,   d2;
   d2   = pop();
   d1   = pop();
   d1.num = (double)(magnitudVector(d1.vect)   !=   0.0 && magnitudVector(d2.vect)   !=  0.0);
   push(d1);
}

void orVV(){
   Datum d1, d2;
   d2 = pop();
   d1 = pop();
   d1.num = (double)(magnitudVector(d1.vect) != 0.0 || magnitudVector(d2.vect) != 0.0);
   push(d1);
}

void notV( ){
   Datum d;
   d = pop();
   d.num = (double)(magnitudVector(d.vect) == 0.0);
   push(d);
}

//ESCALAR-VECTOR
void gtEV() {
   Datum d1,  d2;
   d2 = pop();
   d1 = pop();
   d1.num  =   (double)(d1.num  > magnitudVector(d2.vect));
   push(d1);
}

void ltEV(){
   Datum d1,  d2;
   d2 = pop();
   d1 = pop();
   d1.num  =   (double)(d1.num  < magnitudVector(d2.vect));
   push(d1);
}

void geEV( ) {
   Datum d1,  d2;
   d2  = pop();
   d1  = pop();
   d1.num   =   (double)(d1.num  >= magnitudVector(d2.vect));
   push(d1) ;
}

void leEV() {
   Datum d1,  d2;
   d2   =  pop();
   d1   =  pop();
   d1.num  =   (double)(d1.num   <=  magnitudVector(d2.vect));
   push(d1);
}

void eqEV( ) {
   Datum d1,  d2;
   d2  = pop();
   d1  = pop();
   d1.num  =   (double) (d1.num  ==  magnitudVector(d2.vect));
   push(d1);
}

void neEV(){
   Datum d1, d2;
   d2 = pop();
   d1 = pop();
   d1.num = (double)(d1.num != magnitudVector(d2.vect));
   push(d1);
}

void andEV(){
   Datum d1,   d2;
   d2   = pop();
   d1   = pop();
   d1.num = (double)(d1.num   !=   0.0 && magnitudVector(d2.vect)   !=  0.0);
   push(d1);
}

void orEV(){
   Datum d1, d2;
   d2 = pop();
   d1 = pop();
   d1.num = (double)(d1.num != 0.0 || magnitudVector(d2.vect) != 0.0);
   push(d1);
}

//VECTOR-ESCALAR
void gtVE() {
   Datum d1,  d2;
   d2 = pop();
   d1 = pop();
   d1.num  =   (double)(magnitudVector(d1.vect)  > d2.num);
   push(d1);
}

void ltVE(){
   Datum d1,  d2;
   d2 = pop();
   d1 = pop();
   d1.num  =   (double)(magnitudVector(d1.vect)  < d2.num);
   push(d1);
}

void geVE( ) {
   Datum d1,  d2;
   d2  = pop();
   d1  = pop();
   d1.num   =   (double)(magnitudVector(d1.vect)  >= d2.num);
   push(d1) ;
}

void leVE() {
   Datum d1,  d2;
   d2   =  pop();
   d1   =  pop();
   d1.num  =   (double)(magnitudVector(d1.vect)   <=  d2.num);
   push(d1);
}

void eqVE( ) {
   Datum d1,  d2;
   d2  = pop();
   d1  = pop();
   d1.num  =   (double) (magnitudVector(d1.vect)  ==  d2.num);
   push(d1);
}

void neVE(){
   Datum d1, d2;
   d2 = pop();
   d1 = pop();
   d1.num = (double)(magnitudVector(d1.vect) != d2.num);
   push(d1);
}

void andVE(){
   Datum d1,   d2;
   d2   = pop();
   d1   = pop();
   d1.num = (double)(magnitudVector(d1.vect)   !=   0.0 && d2.num   !=  0.0);
   push(d1);
}

void orVE(){
   Datum d1, d2;
   d2 = pop();
   d1 = pop();
   d1.num = (double)(magnitudVector(d1.vect) != 0.0 || d2.num != 0.0);
   push(d1);
}
