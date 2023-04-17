#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 2



pthread_mutex_t my_mutex;
pthread_cond_t lleno = PTHREAD_COND_INITIALIZER;
pthread_cond_t hay_sitio = PTHREAD_COND_INITIALIZER;

int aforo_actual = 0;
int num_clientes_vip_esperando = 0;
int num_clientes_normales_esperando = 0;
int n_turno_vip = 0;
int n_turno_normal = 0;
int n_turno_global_vip=0;
int n_turno_global_normal=0;

struct thread_args {
    int isvip;
	int id;
};

//productor1
void enter_normal_client(int id)
{	
    pthread_mutex_lock(&my_mutex);
   int miturno=n_turno_normal++;
   printf("turno normal:%d\n",n_turno_normal);

    while (aforo_actual == CAPACITY || num_clientes_vip_esperando > 0 || n_turno_global_normal !=miturno ) {
        num_clientes_normales_esperando++;
        pthread_cond_wait(&hay_sitio, &my_mutex);
        num_clientes_normales_esperando--;
       
    }

   
    aforo_actual++;

    printf("Cliente %d entrando en la discoteca (aforo: %d)\n", id, aforo_actual);
    n_turno_global_normal++;
    //cambia lleno a true
    pthread_cond_broadcast(&hay_sitio);
    pthread_mutex_unlock(&my_mutex);

}

//productor2
void enter_vip_client(int id)
{	
	pthread_mutex_lock(&my_mutex);
    int miturno=n_turno_vip++;
    printf("turno vip:%d\n",n_turno_vip);
    while (aforo_actual == CAPACITY || n_turno_global_vip!= miturno) {
        num_clientes_vip_esperando++;
        pthread_cond_wait(&hay_sitio, &my_mutex);
        num_clientes_vip_esperando--;
    }

   
    aforo_actual++;
    n_turno_global_vip++;
    printf("Cliente %d entrando en la discoteca (aforo: %d)\n", id, aforo_actual);
    pthread_cond_broadcast(&hay_sitio);
    pthread_mutex_unlock(&my_mutex);
}

//sleep
void dance(int id, int isvip)
{   

	printf("Cliente %d dancing in disco\n", id);
	sleep(3);
}

//Consumidor
void disco_exit(int id, int isvip)
{
    pthread_mutex_lock(&my_mutex);
    aforo_actual--;
    printf("Cliente %d saliendo de la discoteca (aforo: %d)\n", id, aforo_actual);
    pthread_cond_broadcast(&hay_sitio);
    pthread_mutex_unlock(&my_mutex);
}


// productor
void *client(void *arg)//Siempre que haya un void como argumento hay que castearlo. Muy importante.
{	
	//Casteo
	struct thread_args *args = (struct thread_args*) arg;
    int isvip = args->isvip;
    int id= args->id;
	if(isvip)
        enter_vip_client(id);

    else
        enter_normal_client(id);

    dance(id,isvip);
    disco_exit(id,isvip);
}

int main(int argc, char *argv[])
{	
	if (argc != 2) {
        printf("Uso: %s <fichero_clientes>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("No se pudo abrir el fichero %s\n", argv[1]);
        return 1;
    }

    int M;
    if (fscanf(f, "%d", &M) != 1) {
        printf("Formato de fichero inválido\n");
        fclose(f);
        return 1;
    }

    int i;
	int result = pthread_mutex_init(&my_mutex, NULL);
    int n_clients=M;
	if (result != 0) {
        printf("Error al inicializar el mutex\n");
		return 1;
    }

	//hilo para cada clinte
	pthread_t threads[n_clients];
    struct thread_args *args=malloc(M*sizeof(struct thread_args));
    
    printf("M: %d\n",M);
	//for que crea M hilos, lee M veces el fichero de entrada
	for(i=0;i<M;i++)
	{	
		//lee el fichero para saber si el cliente es vip o no
		int isvip;
        fscanf(f, "%d", &isvip);

		//argumentos que va a recibir void client*
		args[i].isvip=isvip;
        args[i].id=i;
		//crea un hilo por cada cliente llamando a la funcion client(args)
		//siendo args el identificador de si es vip o no y el mutex global
		int result_hilo= pthread_create(&threads[i],NULL,client,&args[i]);
		if (result != 0) {
            printf("Error al crear el hilo para el cliente %d\n", i);
            return 1;
        }
	}

	// Esperar a que terminen todos los hilos
    for (int i = 0; i < n_clients; i++) {
        pthread_join(threads[i], NULL);
    }

	//destruimos el mutex
	pthread_mutex_destroy(&my_mutex);

	//cerramos el fichero
	fclose(f);

	
	return 0;
}
