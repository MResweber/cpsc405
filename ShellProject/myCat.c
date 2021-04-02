#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * prints any number of files concatenated together to stdout
 */
int main (int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *path = argv[i];               // The path to the current file being read
        FILE *f = fopen(path, "r");

        char ch;                            // The current character being read
        while ((ch = fgetc(f)) != EOF) {
            printf("%c", ch);
        }
    }
}