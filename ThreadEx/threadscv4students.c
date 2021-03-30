#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

//
// Add variables / data structures for condition variables
//


// recursive finonacci O(2**n)
int fib(int i) {
    return (i == 0 || i == 1) ? i : fib(i-1) + fib(i-2);
}

//
// Two threads - computeA and computeB. 
// First thread to run computes fibonacci from 0 to ANSWER_SIZE-1
// First thread subtracts 1 from each fibonacci
// Second thread to run computer fibonacci from ANSWER_SIZE to ANSWER_SIZE+ANSWER_SIZE-1
// Second thread subtracts 1 from each fibonacci
// Both threads wait for the other.
// When one thread is done computing, the other adds 1 to the other's fibonacci
//

#define ANSWER_SIZE 10
int asanswer[ANSWER_SIZE];
int bsanswer[ANSWER_SIZE];

void *computeA(void *arg) {
    int computefirst = (int)arg;
    printf("computeA: %s\n", computefirst ? "A computes values before B." : "A sleeping, computes values after B.");
    if (computefirst) {
        asanswer[0] = 0;
        asanswer[1] = 1;
    }
    else {
        //sleep(1); // sleeping here allows b to finish first
        asanswer[0] = fib(ANSWER_SIZE);
        asanswer[1] = fib(ANSWER_SIZE+1);
    }
    for (int i = 2; i < ANSWER_SIZE; i++)
        asanswer[i] = asanswer[i-1] + asanswer[i-2];
    for (int i = 0; i < ANSWER_SIZE; i++)
        asanswer[i]--;

    // A must signal to B that A is done

    printf("A done computing, A waiting on B\n");

    // A must wait for B to be done

    printf("A computing after B finished.\n");
    for (int i = 0; i < ANSWER_SIZE; i++)
        bsanswer[i]++;

    return NULL;
}

void *computeB(void *arg) {
    int computefirst = (int)arg;
    printf("computeB: %s\n", computefirst ? "B computes values before A." : "B sleeping, computes values after A.");
    if (computefirst) {
        bsanswer[0] = 0;
        bsanswer[1] = 1;
    }
    else {
        //sleep(1); // sleeping here allows a to finish first
        bsanswer[0] = fib(ANSWER_SIZE);
        bsanswer[1] = fib(ANSWER_SIZE+1);
    }
    for (int i = 2; i < ANSWER_SIZE; i++)
        bsanswer[i] = bsanswer[i-1] + bsanswer[i-2];
    for (int i = 0; i < ANSWER_SIZE; i++)
        bsanswer[i]--;

    // B must signal to A that B is done

    printf("B done computing, B waiting on A\n");

    // A must wait for B to be done
    
    printf("B computing after A finished.\n");
    for (int i = 0; i < ANSWER_SIZE; i++)
        asanswer[i]++;

    return NULL;
}

/*

Ways to invoke the program.
% ./a.out
% ./a.out a # can be used to make a finish first
% ./a.out b # can be used to make b finish first

*/

void swap_pointers(void **p1, void **p2) {
    void *t = *p1;
    *p1 = *p2;
    *p2 = t;
}

int main(int argc, char *argv[]) {
    printf("ordering computational threads with condition variables\n");
    int acomputefirst = 1;
    if (argc == 2 && argv[1][0] == 'b')
        acomputefirst = 0; 

    pthread_t p1, p2;

    // Initialize you condition variable data structures

    pthread_create(&p1, NULL, computeA, (void *)(long)acomputefirst);
    pthread_create(&p2, NULL, computeB, (void *)(long)!acomputefirst);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    int *xanswer = asanswer, *yanswer = bsanswer;
    char *xfmt = "asanswer[%d]: %d\n", *yfmt = "bsanswer[%d]: %d\n";
    if (!acomputefirst) {
        swap_pointers((void**)&xanswer, (void**)&yanswer);
        swap_pointers((void**)&xfmt, (void**)&yfmt);
    }
    printf("main printing results of A and B computations.\n");
    for (int i = 0; i < ANSWER_SIZE; i++)
        printf(xfmt, i, xanswer[i]);
    for (int i = 0; i < ANSWER_SIZE; i++)
        printf(yfmt, i, yanswer[i]);
    return 0;
}

// How is the parameter passed to the two threads?
// Why the weird casts on the argument?
// How does the thread access its parameter?
// How does main print the answers in temporal order of threads finishing?
// What allows swap_pointers to swap char* and int*?
