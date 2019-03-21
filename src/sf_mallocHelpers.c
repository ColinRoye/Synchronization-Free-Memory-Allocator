#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "sfmm.h"


sf_block getFreeBlock(size_t requested_size){
    size_t block_size = actualSize(requested_size);
    sf_block current = getNext(sf_free_list_head);
    while(current != sf_free_list_head){
        if(getSize(current) >= size){
            splitBlock(current, block_size, requested_size);
            return current;
        }
        current = getNext(current);
    }
    return NULL;
}
void *getQuickBlock(size_t size){
    return NULL;
}
int actualSize(size_t size){
    size = size + (16-size%16);
    if(size < 32){
        size = 32;
    }
    return size;
}

