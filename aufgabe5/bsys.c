#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    int rd1 = fork();
    int rd2 = fork();

    printf("Hallo\n");

    return 0;
}