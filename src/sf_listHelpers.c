#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "sfmm.h"
#include "debug.h"



unsigned int getBlockSize(sf_block* ptr){
    return (unsigned int)ptr->header.block_size & 0xFFFFFFFC;
}
sf_block *getNextInMem(sf_block* ptr){
    return (sf_block *)(((char*)ptr) + (getBlockSize(ptr)));
}
unsigned int getAlloc(sf_block* ptr){
    debug("nextAlloc:\t%u\n", ptr->header.block_size);
    return (unsigned int) (ptr->header.block_size & 1);

}
unsigned int getPrevAlloc(sf_block* ptr){
    return (unsigned int) (ptr->header.block_size & 0x2) >> 1;
}
unsigned int getNextAlloc(sf_block* ptr){
    debug("nextAlloc:\t%u\n", ptr->header.block_size);
    return getAlloc(getNextInMem(ptr));

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

sf_block *getPrevInMem(sf_block* ptr){
    if(!getPrevAlloc(ptr)){
        char* temp = (char*)ptr;
        unsigned int prev_block_size = getBlockSize((sf_block *)(temp - 8));
        return (sf_block *)(temp - (prev_block_size));
    }
    return NULL;
}

void setPrevAlloc(sf_block* ptr, unsigned int prevAlloc){
     if(prevAlloc > 1){
        debug("WARN: prevAlloc not set!");
        return;
    }
    ptr->header.block_size = (unsigned)(ptr->header.block_size & 0xFFFFFFFD);
    ptr->header.block_size += (prevAlloc << 1);

}
void setAlloc(sf_block* ptr, unsigned int alloc){
    if(alloc > 1){
        debug("WARN: alloc not set!");
        return;
    }
    ptr->header.block_size = (unsigned)(ptr->header.block_size & 0xFFFFFFFE);
    ptr->header.block_size += alloc;
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


int setFooter(sf_block* ptr, unsigned int block_size, unsigned int prevAlloc, unsigned int alloc){
    char *temp = (char *)ptr;
    ptr = (sf_block *)(temp + (block_size -8));
    if(setBlockSize(ptr, block_size) == 0){
        debug("setFooter: bad block size");
        return 0;
    }

    setRequestedSize(ptr, (unsigned int)0);
    setPrevAlloc(ptr, prevAlloc);
    setAlloc(ptr, alloc);
    return 1;
}
int setAllocHeader(sf_block* ptr, unsigned int block_size, unsigned int prevAlloc, unsigned int alloc, unsigned int requested_size){

    if(setBlockSize(ptr, block_size) == 0){
        debug("setFooter: bad block size");
        return 0;
    }
    setRequestedSize(ptr, requested_size);

    setPrevAlloc(ptr, prevAlloc);
    setAlloc(ptr,alloc);
    return 1;
}
// /*wrapper for setFooter, setFooter == setFreeHeader*/
int setFreeHeader(sf_block* ptr, unsigned int block_size, unsigned int prevAlloc, unsigned int alloc, sf_block* prev, sf_block* next){
    if(setBlockSize(ptr, block_size) == 0){
        debug("setFooter: bad block size");
        return 0;
    }
    setRequestedSize(ptr, (unsigned int)0);
    setPrevAlloc(ptr, prevAlloc);
    setAlloc(ptr, alloc);
    setNext(ptr, next);
    setPrev(ptr, prev);
    return 1;
}
void printBlockSize(void* ptr){
    char* temp = (char*)ptr;
    ptr = (sf_block*)(temp - 8);
    printf("BLOCKSIZE: %d\n", getBlockSize(ptr));
}
void clearHeader(sf_block* ptr){
    setRequestedSize(ptr, (unsigned int)0);
    setBlockSize(ptr, (unsigned int)0);
    setPrevAlloc(ptr, (unsigned int)0);
    setAlloc(ptr, (unsigned int)0);

    //setFooter(ptr, (unsigned int)32, (unsigned int)0, (unsigned int)1);
}
sf_block* getFooter(sf_block* ptr){
    char*  temp = (char*)ptr;
    temp += (getBlockSize(ptr)-8);
    return (sf_block*)temp;
}
int isMatch(sf_block* ptr){

    sf_block* ftr = getFooter(ptr);

    if(getBlockSize(ptr) != getBlockSize(ftr)){
        //printf("mismatch bs");
        return 0;
    }
    if(getAlloc(ptr) != getAlloc(ftr)){
        //printf("mismatch na");
        return 0;
    }
    if(getPrevAlloc(ptr) != getPrevAlloc(ftr)){
        //printf("mismatch pa");
        return 0;
    }
    if(getRequestedSize(ptr) != getRequestedSize(ftr)){
        //printf("mismatch rs");
        return 0;
    }
    return 1;
}
void initPrologue(sf_block* ptr){
    char* temp = (char*) ptr;
    ptr = (sf_block*)(temp+8);
    setRequestedSize(ptr, 0);
    setBlockSize(ptr, (unsigned int)32);
    setPrevAlloc(ptr, (unsigned int)0);
    setAlloc(ptr, (unsigned int)1);

    setFooter(ptr, (unsigned int)32, (unsigned int)0, (unsigned int)1);
    isMatch(ptr);


}
void initEpilogue(sf_block* ptr){
    char* temp = (char*)ptr;
    ptr = (sf_block*)(temp + (PAGE_SZ-8));
    setAllocHeader(ptr, 0, 0, 1, 0);
}

void initFreeBlock(sf_block* ptr, sf_block* prev, sf_block* next, unsigned int block_size, unsigned int prevAlloc){
    unsigned int alloc = 0;
    setFreeHeader(ptr, block_size, prevAlloc, alloc, prev, next);


    setFooter(ptr, block_size, prevAlloc, alloc);
    setNext(ptr, next);
    setPrev(ptr, prev);

    // setNext(&sf_free_list_head, ptr);
    // setPrev(ptr, &sf_free_list_head);
}
int initFirstBlock(){
    char* temp = (char*)sf_mem_grow();
    if(temp == NULL){
        sf_errno = ENOMEM;
        return 0;
    }
    sf_block* ptr = (sf_block*)temp;



    initPrologue(ptr);
    initEpilogue(ptr);

    ptr = (sf_block*)(temp + (40));
    initFreeBlock(ptr, &sf_free_list_head, &sf_free_list_head, PAGE_SZ-48, 1);//check alloc bits
    // isMatch(ptr);
    return 0;
}
void clearBlock(sf_block* ptr){
    memset((void*)ptr, 0,  getBlockSize(ptr));
}
unsigned int coaless(sf_block* ptr){
    if(!getPrevAlloc(ptr)){
         sf_block* prevInMem = getPrevInMem(ptr);
        if(setBlockSize(prevInMem, getBlockSize(ptr) + getBlockSize(prevInMem)) == 0){
            return 0;
        }
        //setPrevAlloc(ptr, 0);


        //setPrev(ptr, getPrev(prevInMem));
        //setNext(getPrev(prevInMem), ptr);

        //clearBlock(getPrevInMem(ptr));//can you have 2 free blocks next to eachother?

        setFooter(prevInMem, getBlockSize(ptr), getPrevAlloc(ptr), getAlloc(ptr));
        ptr = prevInMem;
    }
    //ptr =
    // if(!getNextAlloc(ptr)){
    //     if(setBlockSize(ptr, getBlockSize(ptr) + getBlockSize(getNextInMem(ptr))) == 0){
    //         return 0;
    //     }
    //     setAlloc(ptr, 0);

    //     sf_block* nextInMem = getNextInMem(ptr);

    //     //setPrev(getNext(nextInMem), ptr);
    //     setNext(ptr, getNext(nextInMem));

    //     clearBlock(getNextInMem(ptr));

    //     setFooter(ptr, getBlockSize(ptr), getPrevAlloc(ptr), getAlloc(ptr));

    // }
    //setNext(ptr, getNext(&sf_free_list_head));
    setNext(&sf_free_list_head, ptr);
    setPrev(ptr, &sf_free_list_head);

    return 1;
}

unsigned int addPage(){
    char* temp = (char*)sf_mem_grow();
    if(temp == NULL){
        sf_errno = ENOMEM;
        return 0;
    }
    //clear old epilogue
    sf_block* ptr = (sf_block*)(temp-8);
    //clearHeader();
    initEpilogue(ptr);
    ///test heavily
    //setPrev(&sf_free_list_head, ptr);
    initFreeBlock(ptr, &sf_free_list_head, getNext(&sf_free_list_head), (unsigned int)(PAGE_SZ), getAlloc(getPrev(&sf_free_list_head)));//fix
    coaless(ptr);
    return 1;
}
sf_block* splitBlock(sf_block* ptr, unsigned int block_size, unsigned int requested_size, unsigned int total_size){
    unsigned int other_block_size = getBlockSize(ptr) - block_size;
    //check for splunsigned inters
    if(other_block_size < 32){
        return NULL;//deal with
    }
    //get variables for free block
    sf_block* prev = getPrev(ptr);
    sf_block* next = getNext(ptr);

    //unsigned int nextAlloc = (ptr);

    //set allocated block
    setRequestedSize(ptr, requested_size);
    setBlockSize(ptr, block_size);
    setAlloc(ptr, (unsigned int)1);
    //setPrevAlloc(ptr, getPrevAlloc(ptr));
    //set free block
    char* temp = (char*) ptr;
    sf_block* other_ptr = (sf_block *)(temp + getBlockSize(ptr));
    initFreeBlock(other_ptr, prev, next, (unsigned int)((int)total_size - (int)block_size), (unsigned int)1);//err

    setNext(&sf_free_list_head, other_ptr);
    return ptr;

}
void initQuickLists(){
    for(int i = 0; i < NUM_QUICK_LISTS; i++){
        sf_quick_lists[i].length = 0;
        sf_quick_lists[i].first = NULL;
    }

}
int getListBlockSize(int i){
    return (32+(i*16));
}
void emptyQL(int i){
    sf_block* current = sf_quick_lists[i].first;
    for(int j = 0; j < QUICK_LIST_MAX; i++){
        setNext(&sf_free_list_head, current);
        current = getNext(current);
    }
}
void setNextQL(sf_block* ptr, int i){
    if(sf_quick_lists[i].length < QUICK_LIST_MAX){
        if(sf_quick_lists[i].first != NULL){
            ptr->body.links.next = sf_quick_lists[i].first;
            //memset(ptr->body.links.prev, 0, 8);

        }else{
            ptr->body.links.next = NULL;
        }
    } else{
        emptyQL(i);
    }
    //setRequestedSize(ptr,0);
    sf_quick_lists[i].first = ptr;
    ptr->body.links.prev = 0;
}
int addToQuickList(sf_block* ptr){
    for(int i = 0; i < NUM_QUICK_LISTS; i++){
        if(getListBlockSize(i) == getBlockSize(ptr)){
           setNextQL(ptr, i);
           return 1;
        }
    }
    return 0;
}
