#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "sfmm.h"

/*return lsb of header*/
int getNextAlloc(void* ptr){
    return (int)((long int*)(ptr*) & 1);
}
int getPrevAlloc(void* ptr){
    return (int)((long int*)(ptr*) & 2) >> 1;
}
/*return last 32 lsb(s)*/
size_t getBlockSize(void* ptr){
    return (size_t)((long int*)ptr*) & 0xFFFFFFFF;
}

size_t getRequestedSize(void* ptr){
    return (size_t)((long int*)ptr*) >> 32;
}


void setPrevAlloc(void* ptr, int prevAlloc){
    return 1;
}

void setNextAlloc(void* ptr, int nextAlloc){
    return 1;
}

void setBlockSize(void* ptr, size_t block_size){
    return 1;
}

void setRequestedSize(void* ptr, size_t requested_size){
    return 1;
}


void splitBlock(void* ptr, size_t block_size, size_t requested_size){
    int other_block_size = getBlockSize(ptr) - block_size;
    //check for splinters
    if(other_block_size < EFFECTIVE_BLOCK_SIZE){
        return NULL;
    }
    //get variables for free block
    void* prev = getPrevPtr(ptr);
    void* next = getNextPtr(ptr);
    int nextAlloc = getNextAlloc(ptr);

    //set allocated block
    setBlockSize(ptr, requested_size);
    setBlockSize(ptr, block_size);
    setNextAlloc(ptr, 0);
    //set free block
    ptr = (char*) ptr + block_size;
    initFreeBlock(ptr, prev, next, requested_size, block_size, 1, nextAlloc)

}

void setFooter(void* ptr, size_t block_size, int prevAlloc, int nxtAlloc){
    ptr = (char*)ptr + (block_size -8);
    setRequestedSize(ptr, 0);
    setBlockSize(ptr, block_size);
    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr, nextAlloc);
    return 1;
}
/*wrapper for setFooter, setFooter == setFreeHeader*/
void setFreeHeader(void* ptr, size_t block_size, int prevAlloc, int nextAlloc, void* prev, void* next, size_t requested_size){
    setRequestedSize(ptr, 0);
    setBlockSize(ptr, block_size);
    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr, nextAlloc);
    setNext(ptr, next);
    setPrev(ptr, prev);
}
void initFreeBlock(ptr, prev, next, requested_size, block_size, prevAlloc, nextAlloc){
    setFreeHeader(ptr, block_size, prevAlloc, nextAlloc, prev, next, size_t requested_size,)
    setFooter(ptr, block_size, prevAlloc, nxtAlloc)
}
/*Allocated header is the same as set footer, with 2 additional rows (previous in list, next in list)*/
/*NOTE: prev/next in list is not prev/next alloc, it is next in freeList */
void setAllocHeader(void* ptr, size_t block_size, int prevAlloc, int nextAlloc, size_t requested_size){
    setRequestedSize(ptr, requested_size);
    setBlockSize(ptr, block_size);
    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr, nextAlloc);
}