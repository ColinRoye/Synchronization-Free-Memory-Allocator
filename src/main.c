#include <stdio.h>
#include <errno.h>
#include "sfmm.h"
#include "sf_listHelpers.h"

int main(int argc, char const *argv[]) {
    sf_mem_init();

void *x = sf_malloc(3 * PAGE_SZ - sizeof(sf_prologue) - sizeof(sf_epilogue) - 32);    // int* ptr = sf_malloc(sizeof(int));
     sf_show_heap();

    // printBlockSize((void*)ptr);
   // *ptr = 320320320e-320;
    // sf_block* current = &sf_free_list_head;
    // // while(1){
    // //     current = getNext(current);
    // // }
   // printf("%d\n", *ptr);

    sf_free(x);
    // sf_show_heap();
    // sf_free(y);
    // sf_show_heap();

    sf_mem_fini();

    return EXIT_SUCCESS;
}
