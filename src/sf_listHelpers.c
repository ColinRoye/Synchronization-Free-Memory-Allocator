#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "sfmm.h"

void * splitBlock(void* ptr, size_t block_size){
    int other_block_size = getBlockSize(ptr) - block_size;
    //check for splinters
    if(other_block_size < EFFECTIVE_BLOCK_SIZE){
        return NULL;
    }
    //set allocated block
    setBlockSize(ptr, block_size);
    setNextAlloc(ptr, 0);
    //set free block
    ptr = (char*) ptr + block_size;
    setBlockSize

}

int setPrevNextAlloc(void* ptr, int prevAlloc, int nextAlloc){
    return 1;
}

int setBlockSize(void* ptr, size_t block_size){
    return 1;
}

int setRequestedSize(void* ptr, size_t requested_size){
    return 1;
}


int setFooter(void* ptr, size_t block_size, int prevAlloc, int nxtAlloc){
    ptr = (char*)ptr + (block_size -8);
    setRequestedSize(ptr, 0);
    setBlockSize(ptr, block_size);
    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr, nextAlloc);
    return 1;
}
/*wrapper for setFooter, setFooter == setFreeHeader*/
int setFreeHeader(void* ptr, size_t block_size, int prevAlloc, int nextAlloc, void* prev, void* next, size_t requested_size,){
    setRequestedSize(ptr, 0);
    setBlockSize(ptr, block_size);
    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr, nextAlloc);
    setNext(ptr, next);
    setPrev(ptr, prev);

    return 1;
}
int initFreeBlock(void* ptr, void* prev, void* next, size_t requested_size, size_t block_size, int prevAlloc, int nextAlloc){
    setFreeHeader()
}
/*Allocated header is the same as set footer, with 2 additional rows (previous in list, next in list)*/
/*NOTE: prev/next in list is not prev/next alloc, it is next in freeList */
int setAllocHeader(void* ptr, size_t block_size, int prevAlloc, int nextAlloc, size_t requested_size){
    setRequestedSize(ptr, requested_size);
    setBlockSize(ptr, block_size);
    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr, nextAlloc);

    return 1;
}