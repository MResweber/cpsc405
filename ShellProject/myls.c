#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

int main (int argc, char *argv[]){
    char *path;
    if (argc > 1) path = argv[1];
    else path = ".";

    DIR *dir = opendir(path);
    struct dirent *entry = readdir(dir);
    while (entry) {
        if (entry->d_name[0] != '.') printf("%s\n", entry->d_name);
        entry = readdir(dir);
    }
    closedir(dir);
}