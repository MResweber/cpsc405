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
    mq->cur_msgs = 0;
    mq->max_msgs = num_msgs;
    mq->q = NULL;
    sem_t tempEmpty;
    sem_init(&tempEmpty, 0, 0);
    mq->empty = tempEmpty;
    sem_t tempFull;
    sem_init(&tempFull, 0, 0);
    mq->full = tempFull;
    return mq;
}

/*
 * Places m on message queue mq.
 */
int msgq_send(msgq *mq, char *m) {
    // Check if queue is full, if it is wait.
    if(mq->cur_msgs == mq->max_msgs) {
        sem_wait (&mq->full);
    }

    // Assign the data to the heap.
    msg *new_msg = malloc(sizeof(msg));
    new_msg->data = m;

    // Check if the queue is empty if it is jest add the message to the start
    if (mq->q == NULL) {
        mq->q = new_msg;
        mq->cur_msgs++;
        sem_post(&mq->empty);
    }
    
    // Find the end of the queue
    msg *cur = mq->q;
    while (cur->next != NULL) {
        cur = cur->next;
    }

    // Add msg to the end of the queue
    cur->next = new_msg;
    mq->cur_msgs++;
    sem_post(&mq->empty);

    return 1;
}

/*
 * Returns a message from the queue
 */
char *msgq_recv(msgq *mq) {
    // Check if queue is empty, if it is wait.
    if(mq->cur_msgs == 0) {
        sem_wait (&mq->empty);
    }

    // Take the message from the front of the queue
    msg *m = mq->q;
    mq->q = mq->q->next;
    mq->cur_msgs--;
    sem_post(&mq->full);

    char *result = m->data;
    free(m);
    return result;
}