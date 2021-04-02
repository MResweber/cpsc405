#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        char *path = argv[i];
        FILE *f = fopen(path, "r");

        char ch;
        while ((ch = fgetc(f)) != EOF) {
            printf("%c", ch);
        }
    }
}