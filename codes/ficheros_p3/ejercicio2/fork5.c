
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 8
//Modo cutre 
int main(int argc, char **argv)
{
    pid_t pids[N];
    int status;

    pids[0]= getpid();

    // Proceso 2
    pids[1] = fork();
    if (pids[1] == 0) {
        waitpid(pids[0], &status, WUNTRACED);
        printf("Soy el proceso 2 y espero a que termine el proceso %d\n", pids[0]);
        exit(0);
    }

    // Proceso 3
    pids[2] = fork();
    if (pids[2] == 0) {
        waitpid(pids[1], &status, WUNTRACED);
        printf("Soy el proceso 3 y espero a que termine el proceso %d\n", pids[1]);
        exit(0);
    }

    // Proceso 4
    pids[3] = fork();
    if (pids[3] == 0) {
        waitpid(pids[1], &status, WUNTRACED);
        printf("Soy el proceso 4 y espero a que termine el proceso %d\n", pids[1]);
        exit(0);
    }

    // Proceso 5
    pids[4] = fork();
    if (pids[4] == 0) {
        waitpid(pids[0], &status, WUNTRACED);
        printf("Soy el proceso 5 y espero a que termine el proceso %d\n", pids[0]);
        exit(0);
    }

    // Proceso 6
    pids[5] = fork();
    if (pids[5] == 0) {
        waitpid(pids[2], &status, WUNTRACED);
        waitpid(pids[3], &status, WUNTRACED);
        waitpid(pids[4], &status, WUNTRACED);
		waitpid(pids[7], &status, WUNTRACED);
        printf("Soy el proceso 6 y espero a que terminen los procesos %d, %d, %d  y %d\n", pids[2], pids[3], pids[4], pids[7]);
        exit(0);
    }

    // Proceso 7
    pids[6] = fork();
    if (pids[6] == 0) {
        waitpid(pids[0], &status, WUNTRACED);
        waitpid(pids[3], &status, WUNTRACED);
        waitpid(pids[4], &status, WUNTRACED);
        printf("Soy el proceso 7 y espero a que terminen los procesos %d, %d, %d\n",pids[0], pids[3], pids[4]);
		exit(0);
	}

	 // Estamos en el padre
    for (int i = 0; i < N; i++) {
        waitpid(pids[i], NULL, WUNTRACED);
		exit(0);
    }

    return 0;
}