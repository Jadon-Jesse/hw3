/*
  When the mm_malloc function is called,We traverse through the heap testing if the current
  chunk of memory passed will fit in the ith position ... If it does then we simply return the first
  address that it fits in, furthur more if the requested size is much smaller than the newly allocated block sze
  then we split that block into the required size and mark the rest of the block as free... However If we do not 
  find a firstFit then we extend our heap using sbrk by moving the brk and allocating a new block at the end of the 
  list.
  When the mm_free function is called we mark the passed block as free and if the passed block is at the end of our heap
  we move the break back to the address of the previous block to release memory. We also try to prevent fragmantation by fusing 
  empty neighbouring blocks where possible.
  When the mm_realoc function is called we just use the shortcut method by calling mm_maloc and memcpy
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
#include <string.h>



s_block_ptr findFirst(s_block_ptr * lastBlock, size_t size);


struct rlimit limit;
void *base = NULL;

void* mm_malloc(size_t size)
{

    s_block_ptr block, last;

	size_t s;


	s = size;

	if (base) {

	  last = base;
	  block = findFirst (&last ,s);

	  if (block) {

	    if ((block->size - s) >= ( BLOCK_SIZE + 4))
	    split_block (block,s);

	  block->free =0;
	} 

	else {

	  block = extend_heap (last ,s);

	  if (!block)
	    return (NULL );
	  }

	} 

	else {

	  block = extend_heap (NULL ,s);
	
	  if (!block)
	    return (NULL );

	  base = block;
	}

	return (block->data );

}

void* mm_realloc(void* ptr, size_t size)
{
	
    void * re;
    re = mm_malloc(size);
    memcpy(re, ptr, size);

    return re;


}

void mm_free(void* ptr)
{
    s_block_ptr block;

    block = get_block( ptr);
    block->free = 1;

    if( (block ->prev) && (block->prev->free=1) )
    {
    	block = fusion(block->prev);
    }

    if(block -> next)
    {
    	fusion(block);
    }

    else{
    	if(block ->prev)
    		block->prev->next = NULL;
    	else
    		base = NULL;

    	brk(block);
    }
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
	int status;
	s_block_ptr block;

	block = sbrk(0);

	status = (long)sbrk(BLOCK_SIZE + s);

	if(status < 0)
	{
		return NULL;
	}

	block->size = s;
	block->prev = last;
	block->next = NULL;
	block->ptr = block->data;

	if(last)
	{
		last->next = block;
	}
	block->free = 0;

	return block;

}

void split_block (s_block_ptr b, size_t s)
{
	s_block_ptr newBlock;

	newBlock = (s_block_ptr) b->data + s;
	newBlock->size = b->size -s -BLOCK_SIZE;
	newBlock->next = b->next;
	newBlock->prev = b;
	newBlock->free = 1;
	newBlock->ptr = newBlock->data;

	b->size = s;
	b->next = newBlock;

	if(newBlock->next)
	{
		newBlock->next->prev = newBlock;
	}

}

s_block_ptr fusion(s_block_ptr b)
{
  if( (b->next) && (b->next->free=1) )
  {
  	b->size = b->size + BLOCK_SIZE + b->next->size;
  	b->next = b->next->next;

  	if(b->next)
  		b->next->prev = b;
  }

  return b;

}

s_block_ptr get_block (void *p)
{
	char *newAdd;
	newAdd = p;

	newAdd = newAdd - BLOCK_SIZE;

	p = newAdd;

	return p;
}

