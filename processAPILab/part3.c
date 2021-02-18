#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[]){
    int child = fork();
    if (child < 0) printf("Fork Failed\n");
    else if (child == 0) {
        printf("Hello\n");
    }
    else {
        wait(NULL);
        printf("Goodbye\n");
    }
    return 0;
}