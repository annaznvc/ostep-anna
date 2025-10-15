

fd[2];
if(pipe(fd) < 0){
    perror("pipe failed");
    exit(1);
}

int rc1 = pipe();

if (rc1 == 0){
    close(fd[0]);
    
}