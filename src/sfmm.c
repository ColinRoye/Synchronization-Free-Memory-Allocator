
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "debug.h"
#include "sfmm.h"
#include "sf_listHelpers.h"
#include "sf_mallocHelpers.h"


void* sf_malloc(size_t size) {
    unsigned int requested_size = size;
    unsigned int block_size = actualSize(size);
    sf_block* ptr = NULL;
    if(requested_size == 0){
        return NULL;
    }
    if(sf_free_list_head.body.links.next == NULL){//fix
        initQuickLists();
        initFirstBlock();
    }
    if(block_size == -1){
        // return exit failure or NULL?
        return NULL;
    }
    char* temp;

    if((ptr = getQuickBlock(requested_size)) != NULL){
        temp = (char*) ptr;
        temp += 8;
        //memset(temp, 0,(getBlockSize(ptr) - 16));

        return temp;
    }
    if((ptr = getFreeBlock(requested_size)) != &sf_free_list_head){

        temp = (char*) ptr;
        temp += 8;
        //memset(temp, 0,(getBlockSize(ptr) - 16));
        return temp;
    }

    if(addPage() == 0){
        //return exit failure out of mem?
    }
    // if(sf_errno == NOMEM{sear}){
    //     //return exit failure or NULL?
    // }
    return sf_malloc(requested_size);
   // return NULL;
}

void* sf_realloc(void *ptr, size_t size) {
    return NULL;
}
void sf_free(void *ptr) {
    char* chg = (char*)ptr;
    sf_block* temp = (sf_block*)(chg-8);
    if(addToQuickList(temp)){
        return;
    }
    //int check = coaless(temp);
    coaless(temp);
    setNext(temp, &sf_free_list_head);

    return;
}

