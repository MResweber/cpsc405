typedef struct {
    int cur_msgs;
    int max_msgs;
    struct msg *q;
} msgq;

typedef struct {
    char *data;
    struct msg *next;
} msg;

msgq *msgq_init (int num_msgs);
int msgq_send(msgq *mq, char *msg);
char *msgq_recv(msgq *mq);
int msgq_len(msgq *q);
void msgq_show(msgq *mq);