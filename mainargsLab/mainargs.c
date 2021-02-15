#include <stdio.h>

int main(int argc, char **argv) {
    int nflg = 0, sflg = 0, uflg = 0;
    int c = 0;
    for( ; argc>1 && argv[1][0]=='-'; argc--,argv++) {
        c++;
        switch(argv[1][1]) {
        case 0:
            break;
        case 'u':
            printf("-u flag\n");
            uflg++;
            continue;
        case 'n':
            printf("-n flag\n");
            nflg++;
            continue;
            continue;
        case 's':
            printf("-s flag\n");
            sflg++;
            continue;
        default:
            printf("invalid flag\n");
        }
        break;
    }
    printf("Loops: %d, uflags: %d, nflags: %d, sflags: %d\n", c, uflg, nflg, sflg);
}
