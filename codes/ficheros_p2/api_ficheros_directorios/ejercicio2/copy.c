#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>

int copy(int fdo, int fdd)
{
	char *buffer;
    size_t buffer_size;
    size_t bytes_read;
	buffer_size = 512;
    buffer = malloc(buffer_size);
	int n;
	do {
		n = read (fdo,buffer,buffer_size) ;
		if (n > 0)
			n = write (fdd,buffer,n) ;
	} while ( n > 0) ;
	close ( fdo);
	close ( fdd);
	if (n < 0) {
		perror ( " Error en la copia " );
		return -1;
	}
	
	return 0;

}

int main(int argc, char *argv[])
{	
	char*filenameorg= argv[0];
	char *filenamedest = argv[1];
	int fdo,fdd;

	if (fdo = open(filenameorg, O_RDONLY) == -1)
	{
		fprintf(stderr, "No se pudo abrir el fichero de origen\n");
		exit(1);
	}

	if (fdd =open(filenamedest, O_WRONLY) == -1)
	{
		fprintf(stderr, "No se pudo abrir el fichero de destino\n");
		exit(1);
	}

	if(copy(fdo,fdd)==-1)
	{
		fprintf(stderr, "Error en la copia");
		exit(1);

	}
	return 0;
}
	

