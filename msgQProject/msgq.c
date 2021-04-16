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
    mq->q = NULL;

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
    // Wait if the queue is full
    sem_wait (&mq->full);

    // Assign the data to the heap.
    msg *new_msg = malloc(sizeof(msg));
    new_msg->data = m;
    new_msg->next = NULL;

    sem_wait(&mq->mutex);                   // Start of critical region
    // Check if the queue is empty if it is jest add the message to the start
    if (mq->q == NULL) {
        mq->q = new_msg;
        sem_post(&mq->empty);
        sem_post(&mq->mutex);
        return 1;                           // End of critical region
    }

    // Find the end of the queue
    msg *cur = mq->q;
    while (cur->next != NULL) {
        cur = cur->next;
    }

    // Add msg to the end of the queue
    cur->next = new_msg;
    sem_post(&mq->mutex);                   // End of critical region
    sem_post(&mq->empty);

    return 1;
}

/*
 * Returns a message from the queue
 */
char *msgq_recv(msgq *mq) {
    // wait if the queue is empty
    sem_wait (&mq->empty);

    // Take the message from the front of the queue
    msg *m = mq->q;
    mq->q = mq->q->next;
    sem_post(&mq->full);

    char *result = m->data;
    free(m);
    return result;
}

/*
 * Returns number of messages in queue
 */
int msgq_len(msgq *mq) {
    int result = 0;
    msg *curr = mq->q;
    while (curr != NULL){
        result++;
        curr = curr->next;
    }
    
    return result;
}

/*
 * Prints all messages to stdout
 */
void msgq_show(msgq *mq) {
    msg *cur_msg = mq->q;
    while (cur_msg != NULL) {
        printf("%s\n", cur_msg->data);
        cur_msg = cur_msg->next;
    }
}