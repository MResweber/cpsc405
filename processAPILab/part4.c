#include <stdio.h>
#include <unistd.h>

int main (int argc, char *argv[]){
    int child = fork();
    if (child < 0) printf("Fork Failed\n");
    else if (child == 0) {
        printf("####### execv #######\n");
        char *args[]={"/bin/ls", NULL};
        execv(args[0], args);
    }
    else {
        printf("I am you Father\n");
    }
    return 0;
}