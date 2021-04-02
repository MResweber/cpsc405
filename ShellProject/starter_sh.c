#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define DELIMETERS " \t"         // used by strtok, skips whitespace - spaces and tabs
#define BUFSZ 100                   // max size of input line containing commands
#define CMDWORDS 10                 // max words on command line
static char line[BUFSZ];            // cmd line read into line[]
static char *cmd_words[CMDWORDS];   // words on line pointed to by cmd_words[]
                                    // addresses in cmd_words are &line[6] for example
static int num_words = 0;           // number of words on cmd line
static int redirect = 0;            // tracks direction of redirect, -1 is input, 0 is none, and 1 is output
static char *redirectTar;           // holds target of redirect
static int pip = 0;
static char *pipeCmd[CMDWORDS];

/*
 * signal handler for CTL-C
 * CTL-C does not terminate shell, but it does terminate backgroud procs
 */

/*
 * Called when execvp() returns, which happens when the cmd does not exist
 * Prints error message and exits - the exit is from the child, not the shell
 */
void cmd_not_found(char *cmd) {
    fprintf(stderr, "mysh: cmd not found: %s\n", cmd);
    exit(1);                            // exit() from child, not shell
}

/*
 * Checks to see if cmd is cd, and changes directory
 * Returns 1 if the cmd is cd; otherwise returns 0
 */
int cd_cmd() {
    if(line[0] == 'c' && line[1] == 'd' && line[2] == ' '){ // Clumsy 
        if(chdir(line+3) < 0) // Chdir has no effect on the parent if run in the child.
            fprintf(stderr, "cannot cd %s\n", line+3);
        return 1;
    }
    return 0;
}

/*
 * Checks to see if the cmd is exit, and exits if it is
 */
int exit_cmd() {
    if(line[0] == 'e' && line[1] == 'x' && line[2] == 'i' && line[3] == 't')
        exit(1);
    return 0;
}

/*
 * Separates line into words on line
 * For each word on line, cmd_words[] points to the word.
 * num_words assigned the number of words on the line
 * returns 0 for line without command
 */
int get_cmd_words(char *cmd, char *array[]) {
    // Collect words on line into cmd_words
    num_words = 0;
    char *p;
    p = strtok(cmd, DELIMETERS);               // strtok() returns pointer to word on line
    while (p != NULL) {                         // p has address in line, e.g., &line[0]
        array[num_words] = p;               // cmd_words[] points to words on line
        num_words++;                            // count words on line
        p = strtok(NULL, DELIMETERS);           // get next word on line
    }
    array[num_words] = NULL;                // 0 marks end of words in cmd_words
    return num_words;                           // return num of words found
}

void check_pipe(char *cmd) {
    pip = 0;
    char *temp;
    char *pipeLine;
    temp = strtok(cmd, "|");
    temp = strtok(NULL, "|");
    if (temp != NULL) {
        pip = 1;
        pipeLine = temp+1;
    }
    get_cmd_words(pipeLine, pipeCmd);
}
/* 
 * Checks if the cmd line has a redirct in it
 * if it does then it sets it to redirect
 */
void check_redirect(char *cmd) {
    redirect = 0;
    char *temp;
    temp = strtok (cmd, ">");
    temp = strtok (NULL, ">");
    if (temp != NULL) {
        redirect = 1;
        redirectTar = temp+1;
        return;
    }
    temp = strtok (cmd, "<");
    temp = strtok (NULL, "<");
    if (temp != NULL) {
        redirect = -1;
        redirectTar = temp+1;
    }
}

int main() {
    int num = 1, backgroundrun = 0;             // backgroundrun set to 1 after % ./loop &
    while(1) {
        memset(line, 0, BUFSZ);                 // Zero line before each use
        fprintf(stdout, "mysh%d %% ", num);     // display prompt
        fflush(stdout);                         // flush prompt to terminal
        if (fgets(line, BUFSZ, stdin) == 0)     // CTL-D terminates shell
            break;                              // fgets returns LF at end of string
        line[strcspn(line, "\n")] = '\0';       // trim lf from line

        check_pipe(line);
        check_redirect(line);                   // Check if there is a redirect
        if (cd_cmd() || exit_cmd() || 
                !get_cmd_words(line, cmd_words))           // if cd or a blank line
            continue;

        
        if (pip) {
            int p[2];
            pipe(p);
            if (fork() == 0) {
                if (redirect == 1) {
                    int rdfd = open(redirectTar, O_RDWR);
                    dup2 (rdfd, 0);
                }
                dup2(p[1], 1);
                execvp(cmd_words[0], cmd_words);        
                cmd_not_found(cmd_words[0]);
            }
            else if (fork() == 0){
                if (redirect == -1) {
                    int rdfd = open(redirectTar, O_RDWR);
                    dup2 (rdfd, 1);
                }
                dup2(p[0], 0);
                execvp(pipeCmd[0], pipeCmd);        
                cmd_not_found(pipeCmd[0]);
            }
            //else wait(NULL);
        }
        else if (fork() == 0) {
            if (redirect != 0) {
                int rdfd = open(redirectTar, O_RDWR);
                if (redirect == 1) dup2(rdfd, 1);
                else dup2 (rdfd, 0);
            }
            execvp(cmd_words[0], cmd_words);        
            cmd_not_found(cmd_words[0]);        // Successful execvp() does not return
        }
        wait(NULL);
        num++;                                  // command number, blank lines do not increment num
    }
}
