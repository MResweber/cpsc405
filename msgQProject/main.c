#include "msgq.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


msgq *mq;

void producer(void *arg) {
    char *info = (char *) arg;
    char *name = strtok(info, " ");
    char *numString = strtok(NULL, " ");
    int num = atoi(numString);
    for (int i = 0; i < num; i++) {
        char *data = "";
        sprintf(data, "%s: msg num: %d", name, i);
        printf ("%s: Sending: %s\n", name, data);
        msgq_send(mq, data);
    }
}

void consumer(void *arg) {
    char *info = (char *) arg;
    char *name = strtok(info, " ");
    char *numString = strtok(NULL, " ");
    int num = atoi(numString);
    for (int i = 0; i < num; i++) {
        char *data = msgq_recv(mq);
        printf ("%s: recieved: %s\n", name, data);
        free(data);
    }
}

void send_test(void *arg) {
    printf("Sending test msg\n");
    msgq_send(mq, "Test");
    printf("Msg sent\n");
}

void recv_test(void *arg) {
    printf("Reciveing test msg\n");
    char *data = msgq_recv(mq);
    printf("Msg \"%s\" recieved\n", data);
}

int main(int argc, char *argv[]) {
    printf("##########-Testing msgq_init-##########\n");
    mq = msgq_init(5);

    printf("###-Testing msgq_send and msgq_recv-###\n");
    msgq_send(mq, "Hello World");
    char *hello = msgq_recv(mq);
    printf("%s\n", hello);
    
    printf("##########-Testing msgq_len-###########\n");
    int len = msgq_len(mq);
    printf("Length: %d\n", len);
    msgq_send(mq, "Hello World");
    msgq_send(mq, "Hello");
    msgq_send(mq, "World");
    msgq_send(mq, "olleH");
    msgq_send(mq, "dlroW");
    len = msgq_len(mq);
    printf("Length: %d\n", len);
    
    printf("##########-Testing msgq_show-##########\n");
    msgq_show(mq);
    
    printf("#######-Testing msgq_send block-#######\n");
    pthread_t sendBlockTest;
    long t = 0;
    pthread_create(&sendBlockTest, NULL, send_test, (void *)t);
    for (int i = 1; i < 6; i++) {
        sleep(1);
        printf("%d...\n", i);
    }
    msgq_recv(mq);
    sleep(1);
    
    printf("#######-Testing msgq_send block-#######\n");
    msgq_show(mq);
    while (msgq_len(mq) != 0) {
        msgq_recv(mq);
    }
    msgq_show(mq);
    pthread_t recvBlockTest;
    pthread_create(&recvBlockTest, NULL, recv_test, (void *)t);
    for (int i = 1; i < 6; i++) {
        sleep(1);
        printf("%d...\n", i);
    }
    msgq_send(mq, "End Block");
    
    //printf("#######-Producer/Consumer Test-########\n");
    //free(mq);
    //mq = msgq_init(100);
    pthread_exit(NULL);
}