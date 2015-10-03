/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines. Remove this comment and provide
 * a summary of your allocator's design here.
 */

#include "mm_alloc.h"

#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>


/* Your final implementation should comment out this macro. */
#define MM_USE_STUBS


struct rlimit limit;
void *base = NULL;

void* mm_malloc(size_t size)
{

 
	/*
#ifdef MM_USE_STUBS
    return calloc(1, size);
#else
#error Not implemented.
#endif

 
getrlimit(RLIMIT_DATA, &limit);


printf("soft =%llu", (long long) limit.rlim_cur);

#ifdef MM_USE_STUBS
    return calloc(1, size);
#else
#error Not implemented.
#endif
//return 0;*/

    void *point;
    point = sbrk(0);

    if(sbrk(size)== (void *) -1)
    {
    	printf("%s\n", "FAILED");
    	return NULL;
    }
    else{
     point=	sbrk(size);
     printf("%s\n", (char*) &point );
    }

    return point;
}

void* mm_realloc(void* ptr, size_t size)
{
#ifdef MM_USE_STUBS
    return realloc(ptr, size);
#else
#error Not implemented.
#endif
}

void mm_free(void* ptr)
{
#ifdef MM_USE_STUBS
    free(ptr);
#else
#error Not implemented.
#endif
}


s_block_ptr findFirst(s_block_ptr * lastBlock, size_t size)
{
	s_block_ptr block = base;

	while(block && !(block->free && block->size >= size))
	{
		*lastBlock = block;
		block = block->next;
	}

	return block;
}

s_block_ptr extend_heap (s_block_ptr last , size_t s)
{
	s_block_ptr block;
	block = sbrk(0);

	if(sbrk(BLOCK_SIZE + s) == (void *) -1)
	{
		return NULL;
	}
	block->size = s;
	block->next = NULL;

	if(last)
	{
		last->next = block;
	}
	block->free = 0;

	return block;

}