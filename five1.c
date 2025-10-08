//cd /home/pocketlab/ostep-anna
// gcc -o 5one 5one.c
// ./5one



#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int agrc, char *argv[]){

    int x = 100;

    int rc = fork(); //im Kindprozess bekommt man 0, im Elternprozess die PID des Kindes

    if(rc < 0){
        fprintf(stderr, "fork failed");
        exit(1);
    } else if (rc == 0){
        x = 200;
        printf("child variable: %d\n", x);
    } else{
        x = 300;
        printf("parent variable: %d\n", x);
    }


    return 0;
}


// Kind und Eltern sind jeweils zwei getrennte prozesse voneinander und beeinflussen sich nicht