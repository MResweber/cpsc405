#include <semaphore.h>

typedef struct {
    char *data;         // Data of the message
    struct msg *next;   // Next Message in the list
} msg;

typedef struct {    
    msg *q;             // Start of lniked list for queue
    sem_t full;         // Semaphore used to mark the queue as full
    sem_t empty;        // Semaphore used to mark the queue as full
    sem_t mutex;         // Used for exclusion
} msgq;

msgq *msgq_init (int num_msgs);
int msgq_send(msgq *mq, char *msg);
char *msgq_recv(msgq *mq);
int msgq_len(msgq *mq);
void msgq_show(msgq *mq);