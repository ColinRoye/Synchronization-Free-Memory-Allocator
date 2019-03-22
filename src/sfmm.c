
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "debug.h"
#include "sfmm.h"
#include "sf_listHelpers.h"
#include "sf_mallocHelpers.h"


void* sf_malloc(size_t size) {
    size_t requested_size = size;
    size_t block_size = actualSize(size);
    sf_block* ptr = NULL;
    if(requested_size == 0){
        return NULL;
    }
    if(sf_free_list_head.body.links.next == NULL){//fix
        initFirstBlock();
    }
    if(block_size == -1){
        // return exit failure or NULL?
        return NULL;
    }

    if((ptr = getQuickBlock(requested_size)) != NULL){
        return ptr;
    }
    if((ptr = getFreeBlock(requested_size)) != NULL){
        return ptr;
    }

    if(addPage() == -1){
        //return exit failure out of mem?
    }
    // if(sf_errno == NOMEM{sear}){
    //     //return exit failure or NULL?
    // }
    //return sf_malloc(requested_size);
    return NULL;
}

void* sf_realloc(void *ptr, size_t size) {
    return NULL;
}
void sf_free(void *ptr) {
    return;
}

