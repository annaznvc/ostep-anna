#include <stdlib.h> //für exit
#include <stdio.h> 
#include <unistd.h>

int main (int argc, char *argv[]){

    int rc = fork();

    if (rc < 0){
        fprintf(stderr, "fork failed");
    } else if (rc == 0){
        
        printf("Child says hello first\n");
    } else {
        sleep(1);
        printf("Parent says hello later\n");
    }

    return 0;
}