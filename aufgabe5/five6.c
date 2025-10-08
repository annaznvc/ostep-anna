#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

    int rc = fork();

    if(rc < 0){
        fprintf(stderr, "failed fork");
        exit(1);
    } else if (rc == 0){
        printf("Child here\n");
    } else {
        int rc_wait = waitpid(rc, NULL, 0);
        printf("Parent here\n");
    }

    return 0;
}
//waitpid kann auf ein bestimmtes Kind warten
//waitpid nur im Elternprozess, weil wir an Parameter die PID vom Kind brauchen aus rc
// waitpid(pid, status, options);
//pid des kindes, status wie das Programm beendet wurde,
