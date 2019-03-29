#include <stdio.h>
#include <errno.h>
#include "sfmm.h"
#include "sf_listHelpers.h"

int main(int argc, char const *argv[]) {
    sf_mem_init();

    void *x = sf_malloc(sizeof(double) * 8);
    sf_realloc(x, sizeof(int));

    // cr_assert_not_null(x, "x is NULL!");
    // sf_header *hp = (sf_header *)((char *)x - sizeof(sf_header));
    // cr_assert(hp->block_size & THIS_BLOCK_ALLOCATED, "Allocated bit is not set!");
    // cr_assert((hp->block_size & BLOCK_SIZE_MASK) == 48, "Realloc'ed block size not what was expected!");
    sf_show_heap();

    sf_mem_fini();

    return EXIT_SUCCESS;
}
