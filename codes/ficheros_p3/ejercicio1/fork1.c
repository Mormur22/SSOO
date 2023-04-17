#include <stdio.h>
#include <stdlib.h>

/*programa que crea dos hijos: uno no cambia de ejecutable y otro si

	Diseña un programa fork1.c que cree dos procesos hijos. 
	El primero de ellos no cambiará de ejecutable, pero el segundo sí lo hará, mediante una llamada a execvp. 
	El programa recibirá como parámetros el nombre del ejecutable que deberá ejecutar y los argumentos que necesite pasarle.

	El programa realizará una primera llamada a fork. Después de ella, 
	tanto el programa padre como el hijo imprimirán un mensaje indicando si son padre o hijo, su identificador y el de su padre. 
	A continuacion, ambos procesos realizarán una segunda llamada a fork, 
	después de la cual cada proceso imprimirá un mensaje indicando si es padre o hijo, su identificador y el de su padre. 
	Cada hijo generado en la segunda llamada cambiará su ejecutable por el que se haya pasado como argumento al main usando execvp. 
	Cada padre esperará a que sus hijos finalicen.

	El alumno debe consultar las páginas de manual de las siguientes llamadas al sistema: fork, getpid, getppid, execvp, waitpid.

	Para comprobar el funcionamiento correcto de nuestro programa podemos usar como argumento cualquier ejecutable que 
	imprima algo por pantalla, por ejemplo echo o ls.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    pid_t pid1, pid2;
    int status;

    pid1 = fork(); // Primera llamada a fork

    if (pid1 < 0) { // Error al crear el proceso hijo
        fprintf(stderr, "Error al crear el proceso hijo\n");
        exit(1);
    } 
	
	else if (pid1 == 0) { // Código del proceso hijo 1
        printf("Soy el proceso hijo 1 (PID=%d), mi padre es %d\n", getpid(), getppid());

        pid2 = fork(); // Segunda llamada a fork

        if (pid2 < 0) { // Error al crear el proceso hijo
            fprintf(stderr, "Error al crear el proceso hijo\n");
            exit(1);
        } else if (pid2 == 0) { // Código del proceso hijo 2
            printf("Soy el proceso hijo 1.1 (PID=%d), mi padre es %d\n", getpid(), getppid());
            execvp(argv[1], &argv[1]); // Cambio de ejecutable
            fprintf(stderr, "Error al ejecutar el programa\n"); // Si se llega a este punto, ha habido un error al ejecutar el programa
            exit(1);
        } else { // Código del proceso padre 1
            wait(&status); // Espera a que el proceso hijo 2 finalice
            printf("Soy el proceso padre 1 (PID=%d), mi hijo 1.1 ha finalizado con código de retorno %d\n", getpid(), WEXITSTATUS(status));
            exit(0);
        }
    }

	 else { // Código del proceso padre
        printf("Soy el proceso padre (PID=%d), mi hijo es %d\n", getpid(), pid1);

        pid2 = fork(); // Segunda llamada a fork

        if (pid2 < 0) { // Error al crear el proceso hijo
            fprintf(stderr, "Error al crear el proceso hijo\n");
            exit(1);
        } else if (pid2 == 0) { // Código del proceso hijo 3
            printf("Soy el proceso hijo 2 (PID=%d), mi padre es %d\n", getpid(), getppid());
			execvp(argv[1], &argv[1]); // Cambio de ejecutable
            fprintf(stderr, "Error al ejecutar el programa\n"); // Si se llega a este punto, ha habido un error al ejecutar el programa
            exit(1);
        } else { // Código del proceso padre 2
            wait(&status); // Espera a que el proceso hijo 3 finalice
            printf("Soy el proceso padre (PID=%d), mi hijo 2 ha finalizado con código de retorno %d\n", getpid(), WEXITSTATUS(status));
            wait(&status); // Espera a que el proceso hijo 2 finalice
            printf("Soy el proceso padre (PID=%d), mi hijo 1 ha finalizado con código de retorno %d\n", getpid(), WEXITSTATUS(status));
            exit(0);
        }
    }

    return 0;
}
