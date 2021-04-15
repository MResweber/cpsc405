#include "msgq.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("##########-Testing msgq_init-##########\n");
    msgq *mq = msgq_init(100);
    printf("cur_msgs: %d max_msgs: %d\n", mq->cur_msgs, mq->max_msgs);
}