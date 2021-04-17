#include "msgq.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


msgq *mq;
msgq *mq2;

static char *pro1Arr[] = {"pro1: 0", "pro1: 1", "pro1: 2", "pro1: 3", 
    "pro1: 4", "pro1: 5", "pro1: 6", "pro1: 7", "pro1: 8", 
    "pro1: 9", "pro1: 10", "pro1: 11", "pro1: 12", "pro1: 13", 
    "pro1: 14", "pro1: 15", "pro1: 16", "pro1: 17", "pro1: 18",
    "pro1: 19", "pro1: 20", "pro1: 21", "pro1: 22", "pro1: 23", 
    "pro1: 24", "pro1: 25", "pro1: 26", "pro1: 27", "pro1: 28", 
    "pro1: 29", "pro1: 30", "pro1: 31", "pro1: 32", "pro1: 33", 
    "pro1: 34", "pro1: 35", "pro1: 36", "pro1: 37", "pro1: 38", 
    "pro1: 39", "pro1: 40", "pro1: 41", "pro1: 42", "pro1: 43", 
    "pro1: 44", "pro1: 45", "pro1: 46", "pro1: 47", "pro1: 48", 
    "pro1: 49", "pro1: 50"};

static char *pro2Arr[] = {"pro2: 0", "pro2: 1", "pro2: 2", "pro2: 3", 
    "pro2: 4", "pro2: 5", "pro2: 6", "pro2: 7", "pro2: 8", 
    "pro2: 9", "pro2: 10", "pro2: 11", "pro2: 12", "pro2: 13", 
    "pro2: 14", "pro2: 15", "pro2: 16", "pro2: 17", "pro2: 18",
    "pro2: 19", "pro2: 20", "pro2: 21", "pro2: 22", "pro2: 23", 
    "pro2: 24", "pro2: 25", "pro2: 26", "pro2: 27", "pro2: 28", 
    "pro2: 29", "pro2: 30", "pro2: 31", "pro2: 32", "pro2: 33", 
    "pro2: 34", "pro2: 35", "pro2: 36", "pro2: 37", "pro2: 38", 
    "pro2: 39", "pro2: 40", "pro2: 41", "pro2: 42", "pro2: 43", 
    "pro2: 44", "pro2: 45", "pro2: 46", "pro2: 47", "pro2: 48", 
    "pro2: 49", "pro2: 50"};

static char *con1Arr[100];

static char *con2Arr[100];

static char *con3Arr[100];

void *producer(void *arg) {
    char *name;
    name = (char *)arg;
    for (int i = 1; i < 51; i++) {
        if(name[3] == '1') msgq_send(mq2, pro1Arr[i]);
        else msgq_send(mq2, pro2Arr[i]);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    char *name;
    name = (char *)arg;
    int i = 0;
    while(msgq_len(mq2) != 0) {
        char *message = msgq_recv(mq2);
        if (name[3] == '1') con1Arr[i] = message;
        else if (name[3] == '2') con2Arr[i] = message;
        else if (name[3] == '3') con3Arr[i] = message;
        i++;
    }
    pthread_exit(NULL);
}

void *send_test(void *arg) {
    printf("Sending test message\n");
    msgq_send(mq, "Test");
    printf("Message sent\n");
    pthread_exit(NULL);
}

void *recv_test(void *arg) {
    printf("Reciveing test message\n");
    char *data = msgq_recv(mq);
    printf("Message \"%s\" received\n", data);
    pthread_exit(NULL);
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
    // Start the test in a seperate thread
    pthread_t sendBlockTest;
    long t = 0;
    pthread_create(&sendBlockTest, NULL, send_test, (void *)t);
    // Wait 5 seconds while the test is blocked
    for (int i = 1; i < 6; i++) {
        sleep(1);
        printf("%d...\n", i);
    }
    // Recieve a message to end block
    msgq_recv(mq);
    sleep(1);
    
    printf("#######-Testing msgq_send block-#######\n");
    // Clear the queue of messages
    while (msgq_len(mq) != 0) {
        msgq_recv(mq);
    }
    // Confirm that it is empty
    msgq_show(mq);
    // Start the test in a seperate thread
    pthread_t recvBlockTest;
    pthread_create(&recvBlockTest, NULL, recv_test, (void *)t);
    // Wait 5 seconds while the test is blocked
    for (int i = 1; i < 6; i++) {
        sleep(1);
        printf("%d...\n", i);
    }
    // Send a message to end block
    msgq_send(mq, "End Block");
    sleep(1);
    
    printf("#######-Producer/Consumer Test-########\n");
    // Create new attribute for the threads
    pthread_attr_t joinable;
    pthread_attr_init(&joinable);
    pthread_attr_setdetachstate(&joinable, PTHREAD_CREATE_JOINABLE);

    // Create new msgq to prevent any issues from using the previous one
    mq2 = msgq_init(100);

    // Create threads. Currently thread 2 is commented out to isolate problem
    char *pro1arg = "pro1";
    char *pro2arg = "pro2";
    char *con1arg = "con1";
    char *con2arg = "con2";
    char *con3arg = "con3";
    pthread_t pro1;
    pthread_t pro2;
    pthread_t con1;
    pthread_t con2;
    pthread_t con3;
    pthread_create(&pro1, &joinable, producer, (void *)pro1arg);
    pthread_create(&pro2, &joinable, producer, (void *)pro2arg);
    pthread_create(&con1, &joinable, consumer, (void *)con1arg);
    pthread_create(&con2, &joinable, consumer, (void *)con2arg);
    pthread_create(&con3, &joinable, consumer, (void *)con3arg);
    pthread_attr_destroy(&joinable);

    // Wait for the threads to return
    pthread_join(pro1, NULL);
    pthread_join(pro2, NULL);
    pthread_join(con1, NULL);
    pthread_join(con2, NULL);
    pthread_join(con3, NULL);

    // Print the contents of each sonsumer static array
    // This setup is prevent it from attempting to print the NULL pointers
    int i = 0;
    char *curr = con1Arr[0];
    while(curr != NULL) {
        printf("con1: %s\n", curr);
        i++;
        curr = con1Arr[i];
    }
    
    i = 0;
    curr = con2Arr[0];
    while(curr != NULL) {
        printf("con2: %s\n", curr);
        i++;
        curr = con2Arr[i];
    }

    i = 0;
    curr = con3Arr[0];
    while(curr != NULL) {
        printf("con3: %s\n", curr);
        i++;
        curr = con3Arr[i];
    }
    pthread_exit(NULL);
}