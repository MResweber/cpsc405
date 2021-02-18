#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int child = fork();
    if (child < 0) fprintf(stderr, "fork failed\n");
    else if (child == 0) {
        close(1); // close stdout
        int printReturn = printf("hello");
        fprintf(stderr, "printf return: %d\n", printReturn);
    } 
    else { 
        printf("childpid: %d\n", child);
        int wv = wait(NULL);
	assert(wv >= 0);
        printf(" goodbye\n");
        printf("Wait Value: %d\n", wv);
    }
    return 0;
}