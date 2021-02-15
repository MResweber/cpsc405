#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>
/*
Write a program that creates a child process, and then in the child closes standard output (STDOUT FILENO). 
What happens if the child calls printf() to print some output after closing the descriptor?
File Descriptors
  0 - stdin
  1 - stdout
  2 - stderr

 */
int main(int argc, char *argv[]) {
    int rc = fork();
    if (rc < 0) { // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child (new process)
        close(1); // close stdout
        int n = printf("hello");
        fprintf(stderr, "printf return: %d\n", n);
    } else { // parent goes down this path (original process)
        printf("childpid: %d\n", rc);
        int wc = wait(NULL);
	assert(wc >= 0);
        printf(" goodbye\n");
        printf("waitrc: %d\n", wc);
    }
    return 0;
}
