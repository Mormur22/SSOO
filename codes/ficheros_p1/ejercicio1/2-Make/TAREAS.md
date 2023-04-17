1. Examina el makefile, identifica las variables definidas, los objetivos
   (targets) y las regalas.
2. Ejecuta make en la linea de comandos y comprueba las ordenes que ejecuta para
   construir el proyecto.
3. Marca el fichero aux.c como modificado ejecutando touch aux.c. Después
   ejecuta de nuevo make. ¿Qué diferencia hay con la primera vez que lo
   ejecutaste? ¿Por qué?
4. Ejecuta la orden make clean. ¿Qué ha sucedido? Observa que el objetivo clean
   está marcado como phony en la orden .PHONY: clean. ¿por qué? Para comprobarlo
   puedes comentar dicha línea del makefile, compilar de nuevo haciendo make, y
   después crear un fichero en el mismo directorio que se llame clean, usando el
   comando touch clean. Ejecuta ahora make clean, ¿qué pasa?
5. Comenta la linea LIBS = -lm poniendo delante una almoadilla (#). Vuelve a
   contruir el proyecto ejecutando make (haz un clean antes si es necesario).
   ¿Qué sucede? ¿Qué etapa es la que da problemas?
