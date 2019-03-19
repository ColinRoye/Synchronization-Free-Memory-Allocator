#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "sfmm.h"


void *getFreeBlock(size_t size){
    return NULL;
}
void *getQuickBlock(size_t size){
    return NULL;
}
int actualSize(size_t size){
    return -1;
}

void *sf_malloc(size_t size) {
    void ptr;
    if((size = actualSize(size)) == -1){
        //throw error
    }
    if((prt) = getQuickBlock(size)) != NULL){
        return ptr;
    }
    if((prt) = getFreeBlock(size)) != NULL){
        return ptr;
    }
    if(sf_errno
    return NULL;
}