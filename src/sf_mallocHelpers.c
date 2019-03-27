#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "debug.h"
#include "sf_listHelpers.h"

#include "sfmm.h"


int actualSize(unsigned int size){
    size = size + (16-size%16);
    if(size < 32){
        size = 32;
    }
    return size;
}

sf_block* getFreeBlock(unsigned int requested_size){
    unsigned int block_size = actualSize(requested_size);
    sf_block* current = getNext(&sf_free_list_head);
    while(current != &sf_free_list_head){
        if(getBlockSize(current) >= block_size){

            setAlloc(current, 1);
            setRequestedSize(current, requested_size);

            if((getBlockSize(current) - block_size) >= 32){
                current = splitBlock(current, block_size, requested_size, getBlockSize(current));
            }
            break;
        }
        current = getNext(current);
    }

    if(current!=&sf_free_list_head){
        setNextInMemPA(current, 1);
    }
    return current;
}
sf_block* getQuickBlock(unsigned int size){

    ///do this
    return NULL;
}
