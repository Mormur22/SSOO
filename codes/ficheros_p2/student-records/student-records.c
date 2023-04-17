#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
    Esta función lee caracteres del fichero students hasta encontrar el carácter nulo \0. 
    En cada iteración se va realocando memoria para el buffer de la cadena y se van añadiendo los caracteres leídos hasta encontrar el carácter nulo.
     En caso de que se llegue al final del fichero antes de encontrar el carácter nulo, se libera la memoria y se devuelve NULL. 
    Si no hay errores, la función devuelve un puntero a la cadena leída. 
    Es importante recordar que la memoria reservada para la cadena debe ser liberada manualmente cuando ya no se necesite.

*/
char* loadstr(FILE* students) {
    char* str = NULL;
    int len = 0, c;

    /* Read characters until '\0' */
    while ((c = getc(students)) != '\0') {
        if (c == EOF) {
            /* If EOF is found before '\0', free memory and return NULL */
            free(str);
            return NULL;
        }

        /* Reallocate memory for string */
        str = realloc(str, len + 1);
        if (str == NULL) {
            /* If reallocation fails, free memory and return NULL */
            free(str);
            return NULL;
        }
        

        /* Add character to string */
        str[len++] = (char) c;
    }

    /* Add null terminator to string */
    str = realloc(str, len + 1);
    str[len] = '\0';

    return str;
}

int count_students(char *record_list) {
    int num_spaces = 0;

    // Count the number of spaces in the record list
    for (int i = 0; record_list[i] != '\0'; i++) {
        if (record_list[i] == ' ') {
            num_spaces++;
        }
    }

    return num_spaces;
}


// Función para leer una línea del formato "id:nif:nombre:apellido" y devolver una estructura student_t
student_t read_student_line(char* line) {
    student_t student;
    char* token;
    const char delim[2] = ":";

    // Obtener el id del estudiante
    token = strtok(line, delim);
    student.student_id = atoi(token);

    // Obtener el NIF del estudiante
    token = strtok(NULL, delim);
    strncpy(student.NIF, token, MAX_CHARS_NIF);

    // Obtener el nombre del estudiante
    token = strtok(NULL, delim);
    student.first_name = malloc(MAX_CHARS_NAME+1);
    strncpy(student.first_name, token, MAX_CHARS_NAME);

    // Obtener el apellido del estudiante
    token = strtok(NULL, delim);
    student.last_name = malloc(MAX_CHARS_NAME+1);
    strncpy(student.last_name, token, MAX_CHARS_NAME);

	// Add null terminator to strings 								  (added) 
	student.NIF[MAX_CHARS_NIF] = '\0';                               (added) 
	student.firstName[MAX-CHARS-NAME] = '\0';                         (added) 
	student.lastName[MAX-CHARS-NAME] = '\0';                          (added) 
    free(student);
   return student;                                                    (changed) 
}  

student_t* parse_records(char* records[], int nr_records)
{
    struct student_t* students = (struct student_t*) malloc(nr_records * sizeof(struct student_t));
    for (int i = 0; i < nr_records; i++) {
        students[i] = read_student_line(records[i]);
    }
    return students;
}

/*
    Esta función Recibe una lista de estructuras student_t e imprime por pantalla dicha lista con el formato:
        [Entry #0]
            student_id=27
            NIF=67659034X
            first_name=Chris
            last_name=Rock
        [Entry #1]
            student_id=34
            NIF=78675903J
            first_name=Antonio
            last_name=Banderas
*/
void print_students(student_t* students, int n_students) {
    for (int i = 0; i < n_students; i++) {
        printf("[Entry #%d]\n", i);
        printf("\tstudent_id=%d\n", students[i].student_id);
        printf("\tNIF=%s\n", students[i].NIF);
        printf("\tfirst_name=%s\n", students[i].first_name);
        printf("\tlast_name=%s\n", students[i].last_name);
    }
}

/*  La función utiliza la función fwrite para escribir cada registro de estudiante en el fichero binario. 
    El primer argumento de fwrite es un puntero al registro de estudiante que se va a escribir, el segundo argumento es el tamaño de cada registro de estudiante (calculado con sizeof(student_t) en este caso), 
    el tercer argumento es el número de registros de estudiantes que se van a escribir (en este caso, siempre es 1), 
    y el cuarto argumento es el puntero al fichero binario donde se va a escribir el registro.
    La función devuelve el número de registros de estudiantes que se han escrito en el fichero binario. 
    Si ocurre algún error durante la escritura, la función devuelve -1 y muestra un mensaje de error en la salida estándar de error (stderr).
*/
int dump_entries(student_t* entries, int nr_entries, FILE* students) {
    int n_written = 0;
    for (int i = 0; i < nr_entries; i++) {
        int n = fwrite(&entries[i], sizeof(student_t), 1, students);
        if (n != 1) {
            fprintf(stderr, "Error writing student record to file\n");
            return -1;
        }
        n_written++;
    }
    return n_written;
}

student_t* read_student_file(FILE* students, int* nr_entries) {

    // Reservar memoria para el array de registros
    student_t* entries = malloc((*nr_entries) * sizeof(student_t));
    if (entries == NULL) {
        fprintf(stderr, "Error allocating memory for student entries\n");
        return NULL;
    }

    // Leer registros de estudiantes
    for (int i = 0; i < (*nr_entries); i++) {
        int n = fread(&entries[i], sizeof(student_t), 1, students);
        if (n != 1) {
            fprintf(stderr, "Error reading student record from file\n");
            free(entries);
            return NULL;
        }
    }

    return entries;
}

void print_usage(char *program_name) {
    printf("Usage: %s -f file [-h | -l | -c | -a | -q [-i|-n ID]] [list of records]\n", program_name);
    printf("Options:\n");
    printf("  -h               Show this help message and exit\n");
    printf("  -l               List all student records in file\n");
    printf("  -c               Create new student records file\n");
    printf("  -a               Append student records to existing file\n");
    printf("  -q               Query student records by ID or NIF\n");
    printf("  -i ID            Query by student ID\n");
    printf("  -n NIF           Query by student NIF\n");
    printf("Arguments:\n");
    printf("  list of records  Colon-separated list of student records, e.g. \"John:Smith:12345678X\"\n");
}


int create_student_file(char* filename, student_t* records, int nr_records) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Escribir el número de registros en la cabecera del fichero
    fwrite(&nr_records, sizeof(int), 1, file);

    // Escribir los registros de estudiantes en el fichero
    for (int i = 0; i < nr_records; i++) {
        fwrite(&records[i].student_id, sizeof(int), 1, file);
        fwrite(records[i].NIF, sizeof(char), MAX_CHARS_NIF+1, file);
        fwrite(records[i].first_name, sizeof(char), strlen(records[i].first_name)+1, file);
        fwrite(records[i].last_name, sizeof(char), strlen(records[i].last_name)+1, file);
    }

    fclose(file);
    return 0;
}

int list_student_file(char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int nr_records;
    fread(&nr_records, sizeof(int), 1, file);

    printf("Number of records: %d\n", nr_records);
    printf("ID\tNIF\tFirst name\tLast name\n");

    student_t student;
    for (int i = 0; i < nr_records; i++) {
        fread(&student.student_id, sizeof(int), 1, file);
        fread(student.NIF, sizeof(char), MAX_CHARS_NIF+1, file);
        student.first_name = loadstr(file);
        student.last_name = loadstr(file);

        printf("%d\t%s\t%s\t%s\n", student.student_id, student.NIF, student.first_name, student.last_name);

        free(student.first_name);
        free(student.last_name);
    }

    fclose(file);
    return 0;
}

int add_student_record(char* filename, student_t* record) {
    FILE* file = fopen(filename, "ab");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    fwrite(&record->student_id, sizeof(int), 1, file);
    fwrite(record->NIF, sizeof(char), MAX_CHARS_NIF+1, file);
    fwrite(record->first_name, sizeof(char), strlen(record->first_name)+1, file);
    fwrite(record->last_name, sizeof(char), strlen(record->last_name)+1, file);

    fclose(file);
    return 0;
}

student_t* query_student_record(char* filename, int search_id, char* search_NIF) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    int nr_records;
    fread(&nr_records, sizeof(int), 1, file);

    student_t* records = (student_t*) malloc(nr_records * sizeof(student_t));
    if (records == NULL) {
        perror("Error allocating memory"
        return NULL;
    }
}


int main(int argc, char* argv[]) {
    char* filename;
    int create_file = 0;
    int list_file = 0;
    int add_record = 0;
    int query_record = 0;
    int by_id = 0;
    int by_nif = 0;
    int id = 0;
    char nif[MAX_CHARS_NIF+1];

    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_help = 1;
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[i+1];
            i++;
        } else if (strcmp(argv[i], "-l") == 0) {
            query_type = "-l";
        } else if (strcmp(argv[i], "-c") == 0) {
            query_type = "-c";
        } else if (strcmp(argv[i], "-a") == 0) {
            query_type = "-a";
        } else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            query_type = "-q";
            query_arg = argv[i+1];
            i++;
            if (i + 1 < argc && (strcmp(argv[i+1], "-i") == 0 || strcmp(argv[i+1], "-n") == 0)) {
                query_arg = argv[i+2];
                i += 2;
            }
        } else {
            record_list = argv[i];
        }
    }

    if (filename == NULL) {
        printf("Error: missing filename (-f option)\n");
        print_usage();
        return 1;
    }

    if (query_type != NULL) {
        
        switch (query_type)
        {
        case '-l':
            
            

            break;
        case '-c':

            FILE *fp = fopen("filename.bin", "wb");
            if (fp == NULL) {
                perror("Error opening file");
                return EXIT_FAILURE;
            }
            int num_students= count_students(record_list);
            student_t* students = parse_records(record_list,num_students);
            int written_entries = dump_entries(students, num_students, fp);
            printf("%d records written succesfully \n", written_entries);   
            fclose(fp);

            break;

        case '-a':
           
            FILE *fp = fopen("filename.bin", "wb");
            if (fp == NULL) {
                perror("Error opening file");
                return EXIT_FAILURE;
            }
            int num_students= count_students(record_list);
            student_t* students = parse_records(record_list,num_students);
            int written_entries = dump_entries(students, num_students, fp);
            printf("%d records written succesfully \n", written_entries);   
            fclose(fp);

            break;
        case '-q':
            
            if(query_arg!=NULL){

            }
            else{
                 printf("Error: missing query option (-i or -o option)\n");
            }
            break;
        case '-h':
            print_usage();
            break;
        
        default:
            print_usage();
            break;
        }

    }

    return 0;
}