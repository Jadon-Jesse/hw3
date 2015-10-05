/* A simple test harness for memory alloction. */

#include "mm_alloc.h"
#include <stdio.h>
int main(int argc, char **argv)
{
    int *data;
    int *someWord;

    data = (int*) mm_malloc(4);
    data[0] = 1;


    someWord = (int *) mm_malloc(1024);
    someWord[0] = 134;
    someWord[2] = 34;

    mm_realloc((void *) someWord, 1024);


    mm_free(data);

    mm_free(someWord);



    printf("malloc sanity test successful!\n");
    return 0;
}
