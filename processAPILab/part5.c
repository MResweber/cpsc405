#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[]){
    int child = fork();
    if (child < 0) printf("Fork Failed\n");
    else if (child == 0) {
        int wv = wait(NULL);
        printf("Child Wait Value: %d\n", wv);
    }
    else {
        int wv = wait(NULL);
        printf("Parent Wait Value: %d\n", wv);
    }
    return 0;
}