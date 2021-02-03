#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"
#include "book.c"

// Dictionary implementation - simple version
// See https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html?highlight=%28CategoryAlgorithmNotes%29
// for a more complex version
#define MULTIPLIER (37) // nice little prime number
unsigned long hash_function(const char *s) {
    unsigned long h;
    unsigned const char *us = (unsigned const char *)s; // use values >= 0
    h = 0;
    while(*us != '\0') {
        h = h * MULTIPLIER + *us;
        us++;
    } 
    return h;
}

struct elem {
    struct elem *next;
    char *key;
    struct book value;
};
#define DICT_SIZE 31
struct elem *dict[DICT_SIZE];
int num_elems = 0;

void dictput(const char *key, struct book value) {
    struct elem *e;
    unsigned long h;
    e = malloc(sizeof(*e));
    e->key = strdup(key);
    e->value = value;
    h = hash_function(key) % DICT_SIZE;
    e->next = dict[h];
    dict[h] = e;
    num_elems++;
    /* grow table if there is not enough room
    if(num_elems >= DICT_SIZE * MAX_LOAD_FACTOR) {
        grow(d);
    }
    */
}

struct book dictget(const char *key) {
    for (struct elem *e = dict[hash_function(key) % DICT_SIZE]; e != 0; e = e->next) {
        if (!strcmp(e->key, key)) {
            return e->value;
        }
    }
    struct book none;
    none.name = "Not Found";
    none.author = "Not Found";
    return none;
}

void dictprint() {
    printf("**** dict contents ****\n");
    int totelems = 0, totcols = 0;
    for (int i = 0; i < DICT_SIZE; i++) {
        int col = 0;
        for (struct elem *e = dict[i]; e != 0; e = e->next) {
            printf("key: %s, value: %d\n", e->key, e->value);
            totelems++;
            col++;
        }
        if (col > 1)
            totcols = totcols + (col - 1);
    }
    printf("totelems: %d, totcols: %d\n", totelems, totcols);
}