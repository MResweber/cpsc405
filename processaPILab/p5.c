#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    char filename[80];
    if (argc == 2)
        strcpy(filename, argv[1]);
    else {
        printf("enter filename: ");
        scanf("%s", filename);
    }
    int p[2];
    char *execvargv[3];
    execvargv[0] = "wc";
    execvargv[1] = 0; // filled in with data read from pipe
    execvargv[2] = 0;
    pipe(p);
    if(fork() == 0) {
        dup2(p[STDIN_FILENO], STDIN_FILENO); // dup pipe read end on top of stdin
        close(p[STDOUT_FILENO]);
        char buf[100];
        int s = read(p[STDIN_FILENO], buf, 80);
        close(p[STDIN_FILENO]);  // close pipe file descriptors
        buf[s] = 0;
        printf("Child, filename: %s\n", filename);
        execvargv[1] = buf;
        close(STDOUT_FILENO);
        open("./p5output.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU); // redirect stdout to p5output.txt
        execvp(execvargv[0], execvargv);
    } else {
        printf("Parent, filename: %s\n", filename);
        write(p[STDOUT_FILENO], filename, strlen(filename));
        close(p[STDIN_FILENO]);
        close(p[STDOUT_FILENO]);
    }
    return 0;
}
