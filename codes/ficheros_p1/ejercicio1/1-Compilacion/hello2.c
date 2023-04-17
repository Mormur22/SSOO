#include <stdio.h>

/******** TAREAS *****
* 1. Compila el siguiente código y ejecútalo
* 2. Posteriormente, obtén la salida de la etapa de pre-procesado (opción -E o
*    la opción --save-temps para obtener la salida de todas las etapas
*    intermedias) y en un fichero hello2.i
* 3. ¿Qué ha ocurruido con la "llamada a min()" en hello2.i?
* 4. ¿Qué efecto ha tenido la directiva #include <stdio.h>?
*****************/

#define N 5

#define min(x,y) ( (x<y)?x:y )
int a = 7;
int b = 9;
int main() {

 char* cad = "Hello world";
 int i;

 for (i=0;i<N;i++) {
   printf("%s \t a= %d b= %d\n",cad,a,b);
   a++;
   a = min(a,b);
 }
 return 0;
}
