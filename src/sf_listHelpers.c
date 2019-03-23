#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "sfmm.h"
#include "debug.h"


unsigned int getNextAlloc(sf_block* ptr){
    debug("nextAlloc:\t%u\n", ptr->header.block_size);
    return (unsigned int) (ptr->header.block_size & 1);

}
unsigned int getPrevAlloc(sf_block* ptr){
    return (unsigned int) (ptr->header.block_size & 0x2) >> 1;
}
unsigned int getBlockSize(sf_block* ptr){
    return (unsigned int)ptr->header.block_size & 0xFFFFFFFC;
}
unsigned int getRequestedSize(sf_block* ptr){
    //printf("getRequestedSize:\t%u\n",ptr->header.requested_size);
    return (unsigned int) ptr->header.requested_size;
}
sf_block *getNext(sf_block* ptr){
    return ptr->body.links.next;
}
sf_block *getPrev(sf_block* ptr){
    return ptr->body.links.prev;
}
sf_block *getNextInMem(sf_block* ptr){
    if(!getNextAlloc(ptr)){
        return (sf_block *)(((char*)ptr) + (getBlockSize(ptr)));
    }
    return NULL;
}
sf_block *getPrevInMem(sf_block* ptr){
    if(!getPrevAlloc(ptr)){
        unsigned int prev_block_size = getBlockSize((sf_block *)(char*)ptr - 8);
        return (sf_block *)((char*)ptr - prev_block_size);
    }
    return NULL;
}
void setPrevAlloc(sf_block* ptr, unsigned int prevAlloc){
     if(prevAlloc > 1){
        debug("WARN: prevAlloc not set!");
        return;
    }
    ptr->header.block_size = (unsigned)(ptr->header.block_size |(prevAlloc << 1));

}
void setNextAlloc(sf_block* ptr, unsigned int nextAlloc){
    if(nextAlloc > 1){
        debug("WARN: nextAlloc not set!");
        return;
    }
    ptr->header.block_size = (unsigned)(ptr->header.block_size |(nextAlloc));
}
int setBlockSize(sf_block* ptr, unsigned int block_size){
    if(block_size % 16 != 0){
        return 0;
    }
    ptr->header.block_size = block_size; //may break prev and next alloc deppending on order of use
    return 1;
}
void setRequestedSize(sf_block* ptr, unsigned int requested_size){
    ptr->header.requested_size = requested_size;
}
void setPrev(sf_block* ptr, sf_block* prev){
    ptr->body.links.prev = prev;
    prev->body.links.next = ptr;
}
void setNext(sf_block* ptr, sf_block* next){
    ptr->body.links.next = next;
    next->body.links.prev = ptr;
}


int setFooter(sf_block* ptr, unsigned int block_size, unsigned int prevAlloc, unsigned int nextAlloc){
    ptr = (sf_block *)(((char*)ptr) + (block_size -8));
    if(setBlockSize(ptr, block_size) == 0){
        debug("setFooter: bad block size");
        return 0;
    }

    setRequestedSize(ptr, (unsigned int)0);
    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr, nextAlloc);
    return 1;
}
int setAllocHeader(sf_block* ptr, unsigned int block_size, unsigned int prevAlloc, unsigned int nextAlloc, unsigned int requested_size){

    if(setBlockSize(ptr, block_size) == 0){
        debug("setFooter: bad block size");
        return 0;
    }
    setRequestedSize(ptr, requested_size);

    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr,nextAlloc);
    return 1;
}
// /*wrapper for setFooter, setFooter == setFreeHeader*/
int setFreeHeader(sf_block* ptr, unsigned int block_size, unsigned int prevAlloc, unsigned int nextAlloc, sf_block* prev, sf_block* next){
    if(setBlockSize(ptr, block_size) == 0){
        debug("setFooter: bad block size");
        return 0;
    }
    setRequestedSize(ptr, (unsigned int)0);
    setPrevAlloc(ptr, prevAlloc);
    setNextAlloc(ptr, nextAlloc);
    setNext(ptr, next);
    setPrev(ptr, prev);
    return 1;
}
void clearHeader(sf_block* ptr){
    setRequestedSize(ptr, (unsigned int)0);
    setBlockSize(ptr, (unsigned int)0);
    setPrevAlloc(ptr, (unsigned int)0);
    setNextAlloc(ptr, (unsigned int)0);

    //setFooter(ptr, (unsigned int)32, (unsigned int)0, (unsigned int)1);
}
void initPrologue(sf_block* ptr){
    char* temp = (char*) ptr;
    ptr = (sf_block*)(temp+8);
    setRequestedSize(ptr, 0);
    setBlockSize(ptr, (unsigned int)32);
    setPrevAlloc(ptr, (unsigned int)0);
    setNextAlloc(ptr, (unsigned int)1);

    setFooter(ptr, (unsigned int)32, (unsigned int)0, (unsigned int)1);


}
void initEpilogue(sf_block* ptr){
    char* temp = (char*)ptr;
    ptr = (sf_block*)(temp + (PAGE_SZ-8));
    setAllocHeader(ptr, 0, 0, 1, 0);
}
void initFreeBlock(sf_block* ptr, sf_block* prev, sf_block* next, unsigned int block_size, unsigned int prevAlloc, unsigned int nextAlloc){
    setFreeHeader(ptr, block_size, prevAlloc, nextAlloc, prev, next);
    setFooter(ptr, block_size, prevAlloc, nextAlloc);
}
int initFirstBlock(){
    char* temp = (char*)sf_mem_grow();
    if(temp == NULL){
        //sf_errno = ENOMEM;
        return -1;
    }
    sf_block* ptr = (sf_block*)(temp + 8);

    setNext(&sf_free_list_head, ptr);
    //setPrev(&sf_free_list_head, ptr); //unneeded

    initPrologue(ptr);


    ptr = (sf_block*)(temp + (PAGE_SZ - 8));
    initEpilogue(ptr);

    ptr = (sf_block*)(temp + (40));
    initFreeBlock(ptr, &sf_free_list_head, &sf_free_list_head, PAGE_SZ-8, 1, 0);//check alloc bits
    return 0;
}
void clearBlock(sf_block* ptr){
    memset((void*)ptr, 0,  getBlockSize(ptr));
}
unsigned int coaless(sf_block* ptr){
    if(getPrevAlloc(ptr)){

        if(setBlockSize(ptr, getBlockSize(ptr) + getBlockSize(getPrevInMem(ptr))) == 0){
            return 0;
        }
        setPrevAlloc(ptr, 0);

        sf_block* prevInMem = getPrevInMem(ptr);

        setPrev(ptr, getPrev(prevInMem));
        setNext(getPrev(prevInMem), ptr);

        clearBlock(getPrevInMem(ptr));//can you have 2 free blocks next to eachother?

        setFooter(ptr, getBlockSize(ptr), getPrevAlloc(ptr), getNextAlloc(ptr));
    }
    if(getNextAlloc(ptr)){
        if(setBlockSize(ptr, getBlockSize(ptr) + getBlockSize(getNextInMem(ptr))) == 0){
            return 0;
        }
        setNextAlloc(ptr, 0);

        sf_block* nextInMem = getNextInMem(ptr);

        setPrev(getNext(nextInMem), ptr);
        setNext(ptr, getNext(nextInMem));

        clearBlock(getNextInMem(ptr));

        setFooter(ptr, getBlockSize(ptr), getPrevAlloc(ptr), getNextAlloc(ptr));

    }
    return 1;
}

unsigned int addPage(){
    char* temp = (char*)sf_mem_grow();
    if(temp == NULL){
        //sf_errno = ENOMEM;
        return -1;
    }
    //clear old epilogue
    sf_block* ptr = (sf_block*)(temp + (PAGE_SZ - 8));
    initEpilogue(ptr);
    ///test heavily
    ptr = (sf_block*)(temp + (40));
    setPrev(&sf_free_list_head, ptr);
    initFreeBlock(ptr, &sf_free_list_head, getNext(&sf_free_list_head), (unsigned int)(PAGE_SZ - 8), 1, 1);//fix
    coaless(ptr);
    return 1;
}
sf_block* splitBlock(sf_block* ptr, unsigned int block_size, unsigned int requested_size){
    unsigned int other_block_size = getBlockSize(ptr) - block_size;
    //check for splunsigned inters
    if(other_block_size < 32){
        return NULL;
    }
    //get variables for free block
    sf_block* prev = getPrev(ptr);
    sf_block* next = getNext(ptr);
    unsigned int nextAlloc = getNextAlloc(ptr);

    //set allocated block
    setBlockSize(ptr, requested_size);
    setBlockSize(ptr, block_size);
    setNextAlloc(ptr, (unsigned int)0);
    //set free block
    ptr = (sf_block *)(((char*) ptr) + block_size);
    initFreeBlock(ptr, prev, next, block_size, (unsigned int)1, nextAlloc);
    return ptr;

}