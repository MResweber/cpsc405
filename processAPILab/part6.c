#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main (int argc, char *argv[]){
    int child = fork();
    if (child < 0) printf("Fork Failed\n");
    else if (child == 0) {
        printf("Child\n");
    }
    else {
        int status = WNOHANG;
        int wv = waitpid(-1, &status , 0);
        printf("Parent Wait Value: %d\n", wv);
    }
    return 0;
}