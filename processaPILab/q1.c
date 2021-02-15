#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
Write a programthatcallsfork().
Before calling fork(), have the main process access a variable (e.g., x) and set its value to something (e.g., 100). 
What value is the variable in the child process? 
What happens to the variable when both the child and parent change the value of x?
 */

int main(int argc, char *argv[]) {
    int x = 0;
    printf("parent/child modify x (pid:%d)\n", (int) getpid());
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        x += 100;
        printf("child (pid:%d) x = %d\n", (int) getpid(), x);
        sleep(1);
    } else {
        // parent goes down this path (original process)
        int wc = wait(NULL);
        x += 200;
        printf("parent (pid:%d) wc = %d x = %d\n", (int) getpid(), wc, x);
    }
    return 0;
}
