#include "msgq.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("##########-Testing msgq_init-##########\n");
    msgq *mq = msgq_init(100);
    printf("cur_msgs: %d max_msgs: %d\n", mq->cur_msgs, mq->max_msgs);

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
    len = msgq_len(mq);
    printf("Length: %d\n", len);
    
    printf("##########-Testing msgq_show-##########\n");
    msgq_show(mq);
}