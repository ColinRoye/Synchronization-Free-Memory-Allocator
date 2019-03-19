#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "sfmm.h"

int fl_setPrevNextAlloc(vode* ptr, int prevAlloc, int nextAlloc){
    return 1;
}

int fl_setBlockSize(vode* ptr, size_t block_size){
    return 1;
}

int fl_setRequestedSize(vode* ptr, size_t requested_size){
    return 1;
}


int fl_setFooter(void* ptr, size_t requested_size, size_t block_size, int prevAlloc, int nxtAlloc){
    setRequestedSize(ptr, requested_size);
    setBlockSize(ptr, block_size);
    setPrevPostAlloc(ptr, prevAlloc, nxtAlloc);
    return 1;
}
/*wrapper for setFooter, setFooter == setFreeHeader*/
int fl_setFreeHeader(void* ptr, size_t requested_size, size_t block_size, int prevAlloc, int nxtAlloc){
    fl_setFooter(ptr, requested_size, block_size, prevAlloc, nxtAlloc);
    return 1;
}
/*Allocated header is the same as set footer, with 2 additional rows (previous in list, next in list)*/
/*NOTE: prev/next in list is not prev/next alloc, it is next in freeList */
int fl_setAllocHeader(void* ptr, void* prev, void*next, size_t requested_size, size_t block_size, int prevAlloc, int nxtAlloc){
    setFooter(ptr, requested_size, block_size, prevAlloc, nxtAlloc);
    setPrev(ptr, prev);
    setNext(ptr, next);
    return 1;
}