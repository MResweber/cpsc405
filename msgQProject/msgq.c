#include "msgq.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

/*
 * Initializes a message queue and returns a pointer
 * to struct msgq. The parameter num_msgs is the 
 * maximum number of messages that may be in the 
 * message queue.
 */
msgq *msgq_init(int num_msgs) {
    msgq *mq = malloc(sizeof(msgq));
    mq->msgListStart = NULL;

    sem_t tempEmpty;
    sem_init(&tempEmpty, 0, 0);
    mq->empty = tempEmpty;
    
    sem_t tempFull;
    sem_init(&tempFull, 0, num_msgs);
    mq->full = tempFull;
    
    sem_t tempMutex;
    sem_init(&tempMutex, 0, 1);
    mq->mutex = tempMutex;
    return mq;
}

/*
 * Places m on message queue mq.
 */
int msgq_send(msgq *mq, char *m) {
    //printf("Start send\n");
    // Wait if the queue is full
    sem_wait (&mq->full);
    sem_wait(&mq->mutex);
    //printf("Finished waiting\n");
    //if(mq->msgListStart != NULL) printf("Before send list starts with: %s\n", mq->msgListStart->data);
    //else printf("List empty\n");

    // Assign the data to the heap.
    msg *new_msg = malloc(sizeof(msg));
    new_msg->data = m;
    new_msg->next = NULL;
    //printf("Assigned data to heap: %s\n", new_msg->data);

    // Check if the queue is empty if it is jest add the message to the start
    if (mq->msgListStart == NULL) {
        mq->msgListStart = new_msg;
        sem_post(&mq->mutex);
        sem_post(&mq->empty);
        //printf("Added new msg to start of list\n");
        return 1;                           // End of critical region
    }

    // Find the end of the queue
    msg *curr = mq->msgListStart;
    //printf("List starts with: %s\n", curr->data);
    while (curr->next != NULL) {
        curr = curr->next;
        //printf("Next element: %s\n", curr->data);
    }
    //printf("Reached end of list: %s\n", curr->data);

    // Add msg to the end of the queue
    curr->next = new_msg;
    sem_post(&mq->mutex);                   // End of critical region
    sem_post(&mq->empty);
    //printf("finish sending\n");

    return 1;
}

/*
 * Returns a message from the queue
 */
char *msgq_recv(msgq *mq) {
    //printf("Strart receive\n");
    // wait if the queue is empty
    sem_wait (&mq->empty);
    sem_wait(&mq->mutex);
    //printf("finish waiting\n");

    // Take the message from the front of the queue
    msg *m = mq->msgListStart;
    mq->msgListStart = mq->msgListStart->next;
    //printf("Take message\n");

    char *result = m->data;
    free(m);
    sem_post(&mq->mutex);
    sem_post(&mq->full);
    //printf("Return %s\n", result);
    return result;
}

/*
 * Returns number of messages in queue
 */
int msgq_len(msgq *mq) {
    int result = 0;
    sem_wait(&mq->mutex);
    msg *curr = mq->msgListStart;
    while (curr != NULL){
        result++;
        curr = curr->next;
    }
    sem_post(&mq->mutex);
    return result;
}

/*
 * Prints all messages to stdout
 */
void msgq_show(msgq *mq) {
    sem_wait(&mq->mutex);
    msg *cur_msg = mq->msgListStart;
    while (cur_msg != NULL) {
        printf("%s\n", cur_msg->data);
        cur_msg = cur_msg->next;
    }
    sem_post(&mq->mutex);
}