#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "proc.h"
#include "split.h"

/*
 * kernel_proc is the initial process
 * kernel_proc is created by the bootstrap function
 * kernel_proc is created by the following function call
 * kernel_proc = new_proc("kernel_process", 0, 0);
 * kernel_proc has a priority of 0 and will always be the first process in the queue
 */
struct proc *kernel_proc;

/*
 * pid is used to allocate process ids.
 * The pid of kernel_process is 1024
 * pid is incremented as processes are created
 */
int pid = START_PID;

/*
 * state_to_string converts an enum procstate to a printble C string
 */
char *state_to_string(enum procstate ps) {
    switch (ps) {
        case EMBRYO:
            return "EMBRYO";
        case SLEEPING:
            return "SLEEPING";
        case RUNNABLE:
            return "RUNNABLE";
        case RUNNING:
            return "RUNNING";
        case ZOMBIE:
            return "ZOMBIE";
        case UNUSED:
            return "UNUSED";
    }
    return NULL;
}

/*
 * find_proc searches the linked list of procs and returns the struct proc with pid
 * find_proc returns NULL is pid is not in the linked list
 */
struct proc *find_proc(int pid) {
    if(DEBUG) printf("find_proc started\n");
    struct proc *p = kernel_proc;
    while (p->pid != pid){
        if(p->next == NULL) {
    		if(DEBUG) printf("find_proc ended returned: null\n");
			return NULL;
		}
        p = p->next;
    }
    if(DEBUG) printf("find_proc ended returned:");
	if(DEBUG) print_proc(p);
    return p;
}

/*
 * print_proc prints to stdout a struc proc pointer
 */
void print_proc(struct proc *p) {
    if (p == NULL)
        return;
    printf("pname: %s, pid: %d, ppid: %d, priority: %d, state: %s\n",
		 p->name, p->pid, p->parent->pid, p->priority, state_to_string(p->state));
}

/*
 * print_pid prints to stdout the proc corresponding to pid
 */
void print_pid(int pid) {
    struct proc *p = find_proc(pid);
    if (p != NULL)
        print_proc(p);
    else
        printf("pid: %d not found.\n", pid);
}

/*
 * print_procs prints the entire list of procs
 */
void print_procs() {
    printf("procs in queue:\n");
    struct proc *p = kernel_proc;
    do {
        print_proc(p);
        p = p->next;
    } while (p != NULL);
    printf("\n");
}

/*
 * new_proc constructs a struc proc from the heap and returns a pointer to it
 * new_proc has to malloc a struct context and a struct proc
 * Initializing most struct proc members is straight forward.
 * For kernel_proc, p->parent points to kernel_proc.
 * For other procs, you have to search for p->parent in the list.
 */
struct proc *new_proc(char name[], uint priority, int ppid) {
    if(DEBUG) printf("new_proc started\n");
    struct proc *newProc;
    newProc = malloc(sizeof(struct proc));
	strcpy(newProc->name, name);

    struct context *newContext;
	newContext = malloc(sizeof(struct context));
    newProc->context = newContext;

    newProc->priority = priority;

	if(pid == START_PID) newProc->parent = newProc;
    else newProc->parent = find_proc(ppid);

    newProc->pid = pid;
    pid = pid + 1;

    newProc->state = EMBRYO;
    if(DEBUG) printf("new_proc ended returned: ");
	if(DEBUG) print_proc(newProc);
    return newProc;
}

/*
 * enqueue_proc places a proc properly within the linked list of procs based upon the priority.
 */
bool enqueue_proc(struct proc *p) {
    if(DEBUG) printf("enqueue_proc started\n");
    struct proc *current = kernel_proc;
    do {
		if (current->next != NULL) current = current->next;
		else {
			current->next = p;
			p->prev = current;
			return true;
		}
    }while (p->priority > current->priority);
	p->prev = current->prev;
	p->next = current;
    current->prev->next = p;
    current->prev = p;
    if(DEBUG) printf("enqueue_proc ended\n");
    return true;
}

/*
 * bootstrap is called to initialize the doubly linked list of procs
 */ 
void bootstrap() {
    if(DEBUG) printf("bootstrap started\n");
    kernel_proc = new_proc("kernel_process", 0, 0);
    kernel_proc->parent = kernel_proc;
    kernel_proc->state = RUNNABLE;
    if(DEBUG) printf("bootstrap ended\n");
}

/*
 * kill removes a proc from the double linked list based upon pid
 */
bool kill(int pid) {
    if(DEBUG) printf("kill started\n");
    struct proc *p = find_proc(pid);
    if(p->prev != NULL) p->prev->next = p->next;
    if(p->next != NULL) p->next->prev = p->prev;
    free(p);
    if(DEBUG) printf("kill ended\n");
    return true;
}

/*
 * get_pid returns the pid corresponding to a process name. 
 * get_pid returns 0 if the process name is not in the doubly linked list
 */
int get_pid(char name[]) {
    if(DEBUG) printf("get_pid started seraching for: %s\n", name);
    struct proc *p = kernel_proc;
    while (strcmp(p->name, name) != 0){
        p = p->next;
        if(p == NULL){
    		if(DEBUG) printf("get_pid ended returned: 0\n");
			 return 0;
		}
    }
    if(DEBUG) printf("get_pid ended returned: %d\n", p->pid);
    return p->pid;

}

/*
 * fork creates a new process and enqueues it in the doubly linked list
 */
int fork1(char name[], int priority, int ppid) {
    if(DEBUG) printf("fork1 started\n");
    if (priority <= 0)
        return 0;
    struct proc *p = new_proc(name, priority, ppid);
    if (p != NULL)
        if (enqueue_proc(p)) {
            return p->pid;
    		if(DEBUG) printf("fork1 ended\n");
		}
    if(DEBUG) printf("fork1 ended\n");
    return 0;
}
