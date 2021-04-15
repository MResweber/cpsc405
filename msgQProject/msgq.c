#include "msgq.h"
#include <stdio.h>
#include <stdlib.h>

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
    return mq;
}