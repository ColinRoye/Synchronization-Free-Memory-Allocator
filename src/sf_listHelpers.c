#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"


int getNextAlloc(sf_block ptr){
    return (int) (ptr.header.block_size & 1);
}
int getPrevAlloc(sf_block ptr){
    return (int) (ptr.header.block_size & 2) >> 1;
}
size_t getBlockSize(sf_block ptr){
    return (size_t)ptr.header.block_size & 0xFFFFFFFC;
}
size_t getRequestedSize(sf_block ptr){
    return (size_t) ptr.header.block_size;
}
sfblock getNextInMem(sf_block ptr){
    if(!getNextAlloc(ptr)){
        return (sf_block)((char*)ptr + (getBlockSize(ptr)));
    }
}
sfblock getPrevInMem(sf_block ptr){
    if(!getPrevAlloc(ptr)){
        size_t prev_block_size = getBlockSize((sf_block)(char*)ptr - 8);
        return (sf_block)((char*)ptr - prev_block_size);
    }
}
void setPrevAlloc(sf_block ptr, int prevAlloc){
    ptr.header.block_size = (unsigned)(ptr.header.block_size*|(prevAlloc << 1));

}
void setNextAlloc(sf_block ptr, int nextAlloc){
    ptr.header.block_size = (unsigned)(ptr.header.block_size |(nextAlloc << 1));
}
void setBlockSize(sf_block ptr, size_t block_size){
    unsigned prevAlloc = getPrevAlloc(ptr);
    unsigned nextAlloc = getPrevAlloc(ptr);

    ptr.header.block_size = (unsigned)(ptr.header.block_size*);
    setNextAlloc(ptr, nextAlloc);
    setPrevAlloc(ptr, prevAlloc);
}
void setRequestedSize(sf_block ptr, size_t requested_size){
    ptr.header.requested_size = (unsigned)requested_size;
}
void setPrev(sf_block ptr, sf_block prev){
    ptr.header.links.prev = prev
}
void setNext(sf_block ptr, sf_block next){
    ptr.header.links.next = next
}

void splitBlock(sf_block ptr, size_t block_size, size_t requested_size){
    int other_block_size = getBlockSize(ptr) - block_size;
    //check for splinters
    if(other_block_size < EFFECTIVE_BLOCK_SIZE){
        return NULL;
    }
    //get variables for free block
    sf_block prev = getPrevPtr(ptr);
    sf_block next = getNextPtr(ptr);
    int nextAlloc = getNextAlloc(ptr);

    //set allocated block
    setBlockSize(ptr, requested_size);
    setBlockSize(ptr, block_size);
    setNextAlloc(ptr, 0);
    //set free block
    ptr = (char*) ptr + block_size;
    initFreeBlock((sf_block)ptr, prev, next, block_size, 1, nextAlloc)

}
void setFooter(sf_block ptr, size_t block_size, int prevAlloc, int nxtAlloc){
    ptr = (char*)ptr + (block_size -8);
    setRequestedSize(ptr, 0);
    setBlockSize(ptr, block_size);
    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr, nextAlloc);
    return 1;
}
void setAllocHeader(sf_block ptr, size_t block_size, int prevAlloc, int nextAlloc, size_t requested_size){

    setRequestedSize(ptr, requested_size);

    setBlockSize(ptr, block_size);
    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr,nextAlloc)
}
/*wrapper for setFooter, setFooter == setFreeHeader*/
void setFreeHeader(sf_block ptr, size_t block_size, int prevAlloc, int nextAlloc, sf_block prev, sf_block next){
    setRequestedSize(ptr, 0);
    setBlockSize(ptr, block_size);
    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr, nextAlloc);
    setNext(ptr, next);
    setPrev(ptr, prev);
}
void clearHeader(){
    setRequestedSize(ptr, 0);
    setBlockSize(ptr, 0);
    setPrevAlloc(ptr, 0);
    setNextAlloc(ptr, 0);

    setFooter(ptr, 32, 0, 1);
}
void initPrologue(sf_block ptr){;
    setRequestedSize(ptr, 0);
    setBlockSize(ptr, 32);
    setPrevAlloc(ptr, 1);
    setNextAlloc(ptr, 0);

    setFooter(ptr, 32, 0, 1);


}
void initEpilogue(sf_block ptr){
    setAllocHeader(ptr, 0, 0, 1, 0)
}
void initFreeBlock(ptr, prev, next, block_size, prevAlloc, nextAlloc){
    setFreeHeader(ptr, block_size, prevAlloc, nextAlloc, prev, next);
    setFooter(ptr, block_size, prevAlloc, nxtAlloc);
}
void initFirstBlock(){
    char* temp = (char*)sf_mem_grow();
    if(sf_mem_grow == NULL){
        sf_errno = ENOMEM;
        return -1;
    }
    block_size ptr = (block_size)(temp + 8);

    setNext(sf_free_list_head, ptr);
    setPrev(sf_free_list_head, ptr);

    initPrologue(ptr);


    block_size ptr = (block_size)(temp + (PAGE_SZ - 8));
    initEpilogue(ptr);

    block_size ptr = (block_size)(temp + (40));
    initFreeBlock(ptr, sf_free_list_head, sf_free_list_head, block_size, 1, nextAlloc);
}
clearBlock(sf_block ptr){
    memset(ptr, 0,  getBlockSize(ptr));
}
int coaless(sf_block ptr){
    if(getPrevAlloc(ptr)){
        setBlockSize(ptr, getBlockSize(ptr) + getBlockSize(getPrevInMem(ptr)));
        setPrevAlloc(ptr, 0);

        sf_block prevInMem = getPrevInMem(ptr);

        setPrev(ptr, getPrev(prevInMem));
        setNext(getPrev(prevInMem), ptr);

        clearBlock(getPrevInMem(ptr));//can you have 2 free blocks next to eachother?

        setFooter(ptr, getBlockSize(ptr), getPrevAlloc(ptr), getNextAlloc(ptr));
    }
    if(getNextAlloc(ptr)){
        setBlockSize(ptr, getBlockSize(ptr) + getBlockSize(getNextInMem(ptr)));
        setNextAlloc(ptr, 0);

        sf_block nextInMem = getNextInMem(ptr);

        setPrev(getNext(nextInMem), ptr);
        setNext(ptr, getNext(nextInMem));

        clearBlock(getNextInMem(ptr));

        setFooter(ptr, getBlockSize(ptr), getPrevAlloc(ptr), getNextAlloc(ptr));

    }
}

int addPage(){
    char* temp = (char*)sf_mem_grow();
    if(sf_mem_grow == NULL){
        sf_errno = ENOMEM;
        return -1;
    }
    //clear old epilogue
    block_size ptr = (block_size)(temp + (PAGE_SZ - 8));
    initEpilogue(ptr);
    ///test heavily
    block_size ptr = (block_size)(temp + (40));
    sf_free_list_head.links.prev = ptr;
    initFreeBlock(ptr, sf_free_list_head, sf_free_list_head.next, block_size, 1, nextAlloc);
    coaless(ptr);
}