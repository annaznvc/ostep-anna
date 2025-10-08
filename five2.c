#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h> //für open
#include <string.h>

int main(int argc, char *argv[]){

    int fd = open("five2file.txt", O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);

    if (fd == -1) {
        fprintf(stderr, "open failed\n");
        exit(1);
    }

    int rc = fork();

    if (rc < 0){
        fprintf(stderr, "fork failed");
        exit(1);
    } else if (rc == 0){
        printf("child: %d\n", fd);
        write(fd, "child writing\n", strlen("child writing\n"));

    } else {
        printf("parent: %d\n", fd);
        write(fd, "parent writing\n", strlen("parent writing\n"));


    }

    return 0;
}

//beide haben denselben Deskriptor Wert, weil wir open vor dem fork gemacht haben
//beim fork wird der gesamte Prozesszustand kopiert
//wichtig: zwei verweisen auf denselben geöffneten Datei-Eintrag im Kernel
//Wenn jetzt beide gleichzeitig schreiben, beeinflussen sie sich gegenseitig