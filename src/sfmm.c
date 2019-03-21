
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"

void* sf_malloc(size_t size) {
    void* ptr;
    size_t requested_size = size;

    if(requested_size == 0){
        return NULL;
    }
    if(sf_free_list_head == NULL){
        addFirstBlock();
    }
    if((size_t block_size = actualSize(requested_size)) == -1){
        // return exit failure or NULL?
        return NULL;
    }
    if((prt) = getQuickBlock(requested_size) != NULL){
        return ptr;
    }
    if((prt) = getFreeBlock(requested_size) != NULL){
        return ptr;
    }

    // if(addPage() == -1){
    //     //return exit failure out of mem?
    // }
    // if(sf_errno == NOMEM{sear}){
    //     //return exit failure or NULL?
    // }
    return sf_malloc(requested_size);
}

void* sf_realloc(void *ptr, size_t size) {
    return NULL;
}
void sf_free(void *ptr) {
    return;
}