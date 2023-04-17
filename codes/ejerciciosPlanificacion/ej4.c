#define
BLOCK 4096
charbuf[BLOCK] = "xxxxxxx...xxxxx";
voidcopy_block(intfdo, intfdd)
{
    read(fdo, buf, BLOCK);
    write(fdd, buf, BLOCK);
    7
}
voidmain()
{
    pid_tpid;
    intfdo, fdd, i;
    // abre el fichero origen con readonly y le devuelve la entrada 3 de la tfa asociada al fichero y
    // reserva una nueva entrada en la tabla intermedia que contiene el puntero que apunta al siguiente byte de lectura
    fdo = open("Origin", O_RDONLY);
    // abre el fichero destino qu le devuelve la entrada 4 de la tfa y reserva otra nueva entrada en la tabla intermedia que apunta al siguiente byte de escritura/lectura
    //del fichero destino
    fdd = open("Destination", O_RDWR | O_CREAT | O_TRUNC, 0666);

    //este bucle for lo que hace es mediante del uso de lseek sobre las entradas de descriptores de ficheros está desplazando por cada vuelta del buble 
    //desplazando los bloques en el fichero de lectura de uno en uno y en el de escritura de dos en dos para luego leer y escribir donde se supone que quiere el programador.
    //el problema es que el fork se hace después del lseek lo que provoca que los punteros no se actualicen correctamente. Al hacer el fork se clona la tfa del padre que apunta
    //a la misma tabla intermedia que todos los procesos (padres e hijos) en cuyo contenido se van actualizando los punteros de uno en un bloque para el fichero origen y de dos en dos para el fichero destino.
    //Cuando el padre termina de crear todos los procesos el primer hijo lee y escribe bien. Pero el segundo, primero no lee del fichero de orgigen ya que el primer hijo ha dejado el puntero de la tabla intermedia al final
    //del fichero y además buff como no lo comparten entre hijos, va a escribir todo xxxxxx en un bloque nuevo del fichero.
    //el decir lo que hace el programa en sí es escribir d en el útimo bloque inicial del fichero destino y aumentar después su tamaño en 3 bloques habiendo escrito xxx en ellos.

    for (i = 0; i < 4; i++)
    {
        // encuentra el primer bloque donde leer en el fichero origen
        lseek(fdo, i * BLOCK, SEEK_SET);
        lseek(fdd, 2 * i * BLOCK, SEEK_SET);
        pid = fork();
        if (pid == 0)
        {
            copy_block(fdo, fdd);
            exit(0);
        }
    }
    while (wait(NULL) != -1)
    {
    };
    read(fdd, buf, BLOCK);
    seek(fdd, 0, SEEK_SET);
    read(fdd, buf, BLOCK);
}


//Modificación.
#define
BLOCK 4096
charbuf[BLOCK] = "xxxxxxx...xxxxx";

//podríamos llevarnos el lseek al copyblock pero sigue habiendo el mismo problema ya que comparten el el fdd.
/*
    Sin embargo si los propios hijos hacen los respectivos open donde toca de los ficheros solucionaríamos 
    la compartición de los apuntadores de la tabla intermedia. Y ahí ya cada proceso puede leer y escribir donde toca sin joder a los demás procesos.
    Entonces dentro de copy_block haríamos los open, y los lseeks para cada hijo. Ahora bien hay que modificar los parámetros de entrada de la función para que
    sepa que ficheros tiene que abrir y la variable i del bucle para que haga bien los lseek del fichero origen y destino.
*/
void copy_block()
{
    read(fdo, buf, BLOCK);
    write(fdd, buf, BLOCK);
    
}
voidmain()
{
    pid_tpid;
    intfdo, fdd, i;
    
    fdo = open("Origin", O_RDONLY);
    fdd = open("Destination", O_RDWR | O_CREAT | O_TRUNC, 0666);
    for (i = 0; i < 4; i++)
    {
        // encuentra el primer bloque donde leer en el fichero origen
        lseek(fdo, i * BLOCK, SEEK_SET);
        lseek(fdd, 2 * i * BLOCK, SEEK_SET);
        pid = fork();
        if (pid == 0)
        {
            copy_block(fdo, fdd);
            exit(0);
        }
    }
    while (wait(NULL) != -1)
    {
    };
    read(fdd, buf, BLOCK);
    seek(fdd, 0, SEEK_SET);
    read(fdd, buf, BLOCK);
}

/*
    )Indicaelcontenidodelarraybufinmediatamentedespu´esdelaejecuci´ondelasl´ıneas24y26.Justiﬁcaturespuesta.

    )>Realizaelco´digolacopiaintercaladacorrectamente?Enelcasodequenoseaas´ı,escribeunaversi´oncorrectadelacopiaintercaladaparalelaquenohagasuposicionesartiﬁcialesacercadelaplaniﬁcaci´on.
    )SeaelsistemadeﬁcherosdetipoLinux(nodos-i),con3punterosdirectosyunindirectosimple,taman~odebloquede4KiB(4096bytes)y4bytesporpuntero.Dibujaunposible
*/