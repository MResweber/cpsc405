#include <stdio.h>
#include "dict.h"
#include "book.c"

// What is difference between struct strnum1 and struct strnum2?
//In strnum1 the name value is an array of characters, where as strnum2 uses a pointer.
//This means that strnum1 can only have a string with a length less than or equal to 20, but
//strnum2 can have a name value of any length.
struct strnum1 {
  char name[20];
  int num;
};

struct strnum2 {
  char *name;
  int num;
};

// Duplicate keys in the arrays.
// dictput() puts duplicates in What should dictput() do with duplicates?
// dictprint() shows the duplicates; however dictget() returns the first
// These two array generate 18 collisions. How can you reduct collisions?
struct strnum1 strnum1[] = {
    {"Megan", 1}, 
    {"Jacob", 2}, 
    {"Kashif", 3}, 
    {"Thomas", 4}, 
    {"Maryannne", 5}, 
    {"Jacob ", 6}, 
    {"Kelly", 7}, 
    {"Ryan", 8}, 
    {"Grace", 9}, 
    {"Luis", 10}, 
    {"Brittany", 11}, 
    {"Matthew", 12}, 
    {"Sarah", 13}, 
    {"Anthony", 14}
};

struct strnum2 strnum2[] = {
    {"Youseph", 15}, 
    {"Jacob", 16}, 
    {"Alessandro", 17}, 
    {"Joshua", 18}, 
    {"David", 19}, 
    {"Brandon", 20}, 
    {"Megan", 21}, 
    {"Brian", 22}, 
    {"Chy'Nia", 23}, 
    {"Aaron", 24}, 
    {"Charles", 25}, 
    {"Toby", 26}, 
    {"Daniel", 27}, 
    {"Alexander", 28}, 
    {"David", 29}, 
    {"David", 30}, 
    {"Suad", 31}, 
    {"Jessica", 32}, 
    {"James", 33}, 
    {"Lauren", 34}, 
    {"Melody", 35}, 
    {"Miles", 36}, 
    {"Austin", 37}, 
    {"Lauren", 38}
};

int main(int argc, char **argv) {
    for (int i = 0; i < sizeof(strnum1)/sizeof(struct strnum1); i++)
        dictput(strnum1[i].name, strnum1[i].num);

    for (int i = 0; i < sizeof(strnum2)/sizeof(struct strnum2); i++)
        dictput(strnum2[i].name, strnum2[i].num);

    printf("Some dictget() calls.\n");
    printf("key: %s, value: %d\n", "Toby", dictget("Toby"));
    printf("key: %s, value: %d\n", "Brittany", dictget("Brittany"));
    printf("key: %s, value: %d\n", "Megan", dictget("Megan"));
    printf("key: %s, value: %d\n", "David", dictget("David"));
    printf("key: %s, value: %d\n", "Noone", dictget("Noone"));
    printf("\n");

    dictprint();

}