#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    int rc1 = fork();
    if (rc1 < 0) {
        perror("fork failed");
        exit(1);
    }

    if (rc1 == 0) {
        close(fd[0]);                     
        dup2(fd[1], STDOUT_FILENO);       
        close(fd[1]);               

        execlp("ls", "ls", NULL);
        perror("execlp failed");
        exit(1);
    }

    int rc2 = fork();
    if (rc2 < 0) {
        perror("fork failed");
        exit(1);
    }

    if (rc2 == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        execlp("wc", "wc", "-l", NULL);
        perror("execlp failed");
        exit(1);
    }


    close(fd[0]);
    close(fd[1]);
    waitpid(rc1, NULL, 0);
    waitpid(rc2, NULL, 0);

    return 0;
}
