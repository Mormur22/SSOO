#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE* file=NULL;
    int ret;
    unsigned char buffer[1];

    if (argc!=2) {
        fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
        exit(1);
    }

    /* Open file */
    if ((file = fopen(argv[1], "rb")) == NULL)
        err(2,"The input file %s could not be opened",argv[1]);

    /* Read file byte by byte */
    while (fread(buffer, 1, 1, file) == 1) {
        /* Write byte to stdout */
        ret = fwrite(buffer, 1, 1, stdout);

        if (ret != 1){
            fclose(file);
            err(3,"fwrite() failed!!");
        }
    }

    fclose(file);
    return 0;
}
