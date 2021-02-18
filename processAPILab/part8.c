#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int p[2];;
    pipe(p);
    if(fork() == 0) {
        printf("Reader\n");
        dup2(p[1], 1);
        //close(p[0]);
        char *readerString = strcat(argv[1], "World");
        printf("%s\n", readerString);
        write(p[1], readerString, strlen(readerString));
        close(p[1]);
        
    } 
    else {
        if (fork() == 0) {
            printf("Writer\n");
            dup2(p[0], 0);
            close(p[0]);
            char *writerString;
            scanf("%s\n", writerString);
            printf("%s", writerString);
            close(p[1]);
            char *revArg;
            for (int i = 0; i < strlen(argv[1]); i++) {
                strcat(revArg, " ");
            }
            int x = 0;
            for (int i = strlen(argv[1]); i >= 0; i--) {
                revArg[x] = argv[1][i];
                x++;
            }
            printf("%s%s", writerString, revArg);
        }
        else {
            printf("Parent\n");
            close(p[0]);
            close(p[1]);
            wait(NULL);
            wait(NULL);
        }
    }
    return 0;
}