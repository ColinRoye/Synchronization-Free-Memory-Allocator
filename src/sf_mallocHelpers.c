#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "debug.h"
#include "sf_listHelpers.h"


#include "sfmm.h"


int actualSize(unsigned int size){
    unsigned int requested_size = size;
    size = size + (16-size%16);
    if(size < 32){
        size = 32;
    }
    if((size-requested_size) < 8){
        return (size + 16);
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
        setAlloc(current, 1);
       // FL_remove(current);
        setNextInMemPA(current, 1);
    }
    return current;
}
int isQuick(unsigned int block_size){
    unsigned int num = block_size - 32;
    num = num / 16;
    if(num < (NUM_QUICK_LISTS)){
        return num;
    }
    return -1;
}
sf_block* getQuickBlock(unsigned int requested_size){
    unsigned int block_size = actualSize(requested_size);
    int i;
    if((i = isQuick(block_size)) != -1){
        if(sf_quick_lists[i].length > 0){
            sf_quick_lists[i].length -= 1;
            sf_block* ptr = sf_quick_lists[i].first;
            setRequestedSize(ptr, requested_size);
            if(sf_quick_lists[i].length == 0){
                sf_quick_lists[i].first = NULL;

            }else{
                sf_quick_lists[i].first = getNext(sf_quick_lists[i].first);
            }
            return ptr;
        }
    }
    return NULL;
}
