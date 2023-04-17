#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NUMITER 3

int getServingsFromPot(void)
{

}

void eat(void)
{
	unsigned long id = (unsigned long) getpid();
	printf("Savage %lu eating\n", id);
	sleep(rand() % 5);
}

void savages(void)
{
	for (i = 0; i < NUMITER; i++){
		//comprueba que el Pot tiene una ración para él.
		// si resulta que está vacio despierta al semáforo compartido y el cocinero
		//se pone a cocinar
  		getServingFromPot()
  		eat()
	}
}

int main(int argc, char *argv[])
{

	return 0;
}
