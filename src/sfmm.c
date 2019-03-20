
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"

void* sf_malloc(size_t size) {
    void ptr;
    if((size = actualSize(size)) == -1){
        //throw error
    }
    if((prt) = getQuickBlock(size) != NULL){
        return ptr;
    }
    if((prt) = getFreeBlock(size) != NULL){
        return ptr;
    }

    addPage();
    //     if(sf_errno == NOMEM{sear}){
//         return NULL;
//     }
    return sf_malloc(size);
}

void* sf_realloc(void *ptr, size_t size) {
    return NULL;
}
void sf_free(void *ptr) {
    return;
}