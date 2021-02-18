#include <stdio.h>
#include <unistd.h>

int main (int argc, char *argv[]){
    int x = 100;
    int child = fork();
    if (child < 0) printf("Fork Failed\n");
    else if (child == 0) {
        x += 1;
        printf("Child Value: %d\n", x);
    }
    else {
        x += 2;
        printf("Parent Value: %d\n", x);
    }
    return 0;
}