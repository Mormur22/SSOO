int a = 3;
void main()
{
    int b = 2;
    int p;
    for (i = 0; i < 4; i++)
    {
        p = fork();
        if (p == 0)
        {
            b++;
            execlp("command": ...);
            a++;
        }
        else
        {   
            //
            wait();
            a++;
            b--;
        }
    }
    printf("a= %d,b= %d\n", a, b);
}

/*


a) ¿Cuántos procesos se crean en total? (sin contar el padre original) ¿Cuántos coexisten en
el sistema como máximo? Como máximo coexisten 2 procesos. El padre antes de crear un hijo nuevo espera a la terminación del proceso hijo anterior.
b) ¿Qué imprimiría por pantalla la última sentencia del programa? Imprimiría los valores de a y b del proceso padre, es decir 7 y -2. 




*/