#include <stdio.h>
#include <fcntl.h> 
#include <unistd.h>
extern int errno; 

int main (int argc, char *argv[]){
    //int rc = fork();
    int file = open("~/cpsc405Repo/processAPILab/test.txt", O_RDONLY | O_CREAT);
    printf("File Result Parent: %d\n", file);
    if (file ==-1) { 
        // print which type of error have in a code 
        printf("Error Number % d\n", errno);  
          
        // print program detail "Success or failure" 
        perror("Program");                  
    } 
    return 0;
}