#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"


int getNextAlloc(sf_block* ptr){
    return (int) (*ptr.header.block_size & 1);
}
int getPrevAlloc(sf_block* ptr){
    return (int) (*ptr.header.block_size & 2) >> 1;
}
size_t getBlockSize(sf_block* ptr){
    return (size_t)ptr.header.block_size & 0xFFFFFFFC;
}
size_t getRequestedSize(sf_block* ptr){
    return (size_t) ptr.header.block_size;
}

void setPrevAlloc(sf_block* ptr, int prevAlloc){
    ptr.header.block_size* = (unsigned)(ptr.header.block_size*|(prevAlloc << 1));

}
void setNextAlloc(sf_block* ptr, int nextAlloc){
    ptr.header.block_size* = (unsigned)(ptr.header.block_size*|(nextAlloc << 1));
}
void setBlockSize(sf_block* ptr, size_t block_size){
    unsigned prevAlloc = getPrevAlloc(ptr);
    unsigned nextAlloc = getPrevAlloc(ptr);

    ptr.header.block_size* = (unsigned)(ptr.header.block_size*);
    setNextAlloc(ptr, nextAlloc);
    setPrevAlloc(ptr, prevAlloc);
}
void setRequestedSize(sf_block* ptr, size_t requested_size){
    ptr.header.requested_size = (unsigned)requested_size;
}
void setPrev(sf_block* ptr, sf_block* prev){
    ptr.header.links.prev = prev
}
void setNext(sf_block* ptr, void* next){
    ptr.header.links.next = next
}

void splitBlock(sf_block* ptr, size_t block_size, size_t requested_size){
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
void setFooter(sf_block* ptr, size_t block_size, int prevAlloc, int nxtAlloc){
    ptr = (char*)ptr + (block_size -8);
    setRequestedSize(ptr, 0);
    setBlockSize(ptr, block_size);
    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr, nextAlloc);
    return 1;
}
/*wrapper for setFooter, setFooter == setFreeHeader*/
void setFreeHeader(sf_block* ptr, size_t block_size, int prevAlloc, int nextAlloc, void* prev, void* next, size_t requested_size){
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
void setAllocHeader(sf_block* ptr, size_t block_size, int prevAlloc, int nextAlloc, size_t requested_size){
    setRequestedSize(ptr, requested_size);
    setBlockSize(ptr, block_size);
    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr, nextAlloc);
}