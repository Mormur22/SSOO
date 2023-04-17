#include <stdio.h>
#include <unistd.h>
int main()
{
    char name[20];
    printf("Enter your Name\n");
    scanf("%s",&name);
    printf("\nYour name is = %s \n",name);
    pause();
    return 0;
}
