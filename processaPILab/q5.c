#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
/*
Now write a program that uses wait() to wait for the child process to finish in the parent. 
What does wait() return? 
What happens if you use wait() in the child?
 */
int main(int argc, char *argv[]) {
    int rc = fork();
    if (rc < 0) { // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child (new process)
        printf("hello");
    } else { // parent goes down this path (original process)
        printf("childpid: %d\n", rc);
        int wc = wait(NULL);
	assert(wc >= 0);
        printf(" goodbye\n");
        printf("waitrc: %d\n", wc);
    }
    return 0;
}
