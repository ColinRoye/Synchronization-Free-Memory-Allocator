#include <stdio.h>
#include <errno.h>
#include "sfmm.h"
#include "sf_listHelpers.h"

int main(int argc, char const *argv[]) {
    sf_mem_init();

    sf_errno = 0;
    void *x = sf_malloc(3 * PAGE_SZ - sizeof(sf_prologue) - sizeof(sf_epilogue) - 32);
    sf_show_heap();
    if(x!=x){
        printf("NOP");
    }
    sf_mem_fini();

    return EXIT_SUCCESS;
}
