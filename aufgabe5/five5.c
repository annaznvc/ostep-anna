#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "failed fork\n");
        exit(1);
    } else if (rc == 0) {
        int rc_wait = wait(NULL);
        printf("Child here\n");
        printf("Child here (rc_wait=%d)\n", rc_wait);
    } else {
        //int rc_wait = wait(NULL);
        printf("Parent here\n");
    }

    return 0;
}
