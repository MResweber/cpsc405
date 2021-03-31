#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

struct list {
    struct list *next;
    int val;
};

struct list *head;
struct list *tail;

void printqueue() {
    if (head == NULL)
        printf("queue is empty\n");
    else {
        printf("queue values:\n");
        struct list *l = head;
        while (l != NULL) {
            printf("%d\n", l->val);
            l = l->next;
        }
    }
}

void initqueue() {
    head = tail = NULL;
}

int enqueue(int val) {
    struct list *t = malloc(sizeof(struct list)); // could return -1 if malloc fails
    t->val = val;
    t->next = NULL;
    //if (tail == NULL || head == NULL)
    if (head == NULL)
        tail = head = t;
    else {
        tail->next = t;
        tail = t;
    }
    return 0;
}

struct list *dequeue() {
    struct list *retval = NULL;
    if (head != NULL) {
        retval = head;
        head = head->next;
    }
    return retval;
}

pthread_mutex_t m;
int loops;

/*
Iterate loops times and enqueue a different number on each iteration.
Properly guard the critical region
Print the value enqueued. 
Place the print outside of the critical region.
You can sleep on each iteration to slow down.
Place sleep outside of critical region
 */
void *worker1(void *arg) {
    pthread_exit(NULL);
}

/*
Iterate loops times and dequeue a value on each iteration.
Properly guard the critical region
If the value dequeued is NULL, print queue empty; otherwise print the value dequeued. 
Place the print outside of the critical region.
You can sleep on each iteration to slow down.
Place sleep outside of critical region
 */
void *worker2(void *arg) {
    pthread_exit(NULL);
}

int
main(int argc, char *argv[]) {
    if (argc != 2) { 
	fprintf(stderr, "usage: threads <value>\n"); 
	exit(1); 
    } 
    loops = atoi(argv[1]);
    initqueue();
    enqueue(1);
    enqueue(111);
    printqueue();
    printf("dequeue: %d\n", dequeue()->val);
    enqueue(222);
    printqueue();
    printf("dequeue: %d\n", dequeue()->val);
    printf("dequeue: %d\n", dequeue()->val);
    printqueue();

    /*
    Create two threads. One is worker1 and the other is worker2
    Create the proper variable to guard the critical region.
    Wait for the two threads to finish.
     */
    printqueue();
    return 0;
}

