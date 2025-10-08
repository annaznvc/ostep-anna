#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

    int rc = fork();

    if (rc < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0){
        
        // Argumente als Liste, vollständiger Pfad erforderlich
        // execl("/bin/ls", "ls", "-l", NULL);
        
        // Wie execl(), aber mit eigenem Environment
        // char *envp[] = { "PATH=/bin:/usr/bin", "USER=student", NULL };
        // execle("/bin/ls", "ls", "-l", NULL, envp);
        
        // Argumente als Liste, sucht in PATH
        // execlp("ls", "ls", "-l", NULL);
        
        // Argumente als Array, vollständiger Pfad erforderlich
        // char *args[] = { "ls", "-l", NULL };
        // execv("/bin/ls", args);
        
        // Argumente als Array, sucht in PATH
        // char *args[] = { "ls", "-l", NULL };
        // execvp("ls", args);
        
        // Argumente als Array, sucht in PATH, eigenes Environment
        char *args[] = { "ls", "-l", NULL };
        char *envp[] = { "PATH=/bin:/usr/bin", "USER=student", NULL };
        execvpe("ls", args, envp);
        
        perror("exec failed");
        exit(1);
        
    } else {
        int rc_wait = wait(NULL);
        printf("Hello parent!\n");
    }

    return 0;
}