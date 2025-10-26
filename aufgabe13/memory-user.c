#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]){


    if (argc > 2 || argc == 1){
        fprintf(stderr, "anzahl argumente falsch\n");
        exit(1);
    }

    int pid = getpid();
    printf("PID: %d\n", pid);

    long long argument = atoll(argv[1]) * 1024 * 1024;

    char* array;
    array = (char*) malloc(argument);

    if (array == NULL){
        fprintf(stderr,"Malloc failed\n");
        exit(1);
    }

   while (1) {
        for (long long i = 0; i < argument; i++) {
            array[i] += 1;
        }
    }

    free(array);

    return 0;
}