#include "msgq.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


msgq *mq;
msgq *mq2;

static char *con1Arr[100];
static char *con2Arr[100];
static char *con3Arr[100];

void producer(void *arg) {
    char *name;
    name = (char *)arg;
    printf("Check thread name: %s\n", name);
    for (int i = 1; i < 51; i++) {
        printf("Check for loop start: %s %d\n", name, i);
        char *data;
        sprintf(data, "Msg:%d: Sent by: %s", i, name);
        printf("Check format data \"%s\", %s\n", data, name);
        msgq_send(mq2, data);
        printf("Sent Sucessfully\n");
    }
    printf("End of for loop\n");
    pthread_exit(NULL);
}

void consumer(void *arg) {
    char *name = (char *) arg;
    int i = 0;
    while (msgq_len(mq2) != 0) {
        char *data = msgq_recv(mq2);
        char *m = "";
        sprintf (m, "%s: Recieved by: %s", data, name);
        if(name[3] == '1') con1Arr[i] = m;
        else if(name[3] == '2') con2Arr[i] = m;
        else if(name[3] == '3') con3Arr[i] = m;
        i++;
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
    /*
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
    sleep(1);
    */
    printf("#######-Producer/Consumer Test-########\n");
    printf("Start\n");
    pthread_attr_t joinable;
    pthread_attr_init(&joinable);
    pthread_attr_setdetachstate(&joinable, PTHREAD_CREATE_JOINABLE);
    mq2 = msgq_init(100);
    char *pro1arg = "pro1";
    char *pro2arg = "pro2";
    pthread_t pro1;
    pthread_t pro2;
    pthread_create(&pro1, &joinable, producer, (void *)pro1arg);
    printf("Check thread 1\n");
    pthread_create(&pro2, &joinable, producer, (void *)pro2arg);
    printf("Check thread 2\n");
    pthread_attr_destroy(&joinable);
    pthread_join(pro1, NULL);
    printf("Check thread 1 join\n");
    pthread_join(pro2, NULL);
    printf("Check thread 2 join\n");
    msgq_show(mq2);
    pthread_exit(NULL);
}