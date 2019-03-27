#include <stdio.h>
#include <errno.h>
#include "sfmm.h"
#include "sf_listHelpers.h"

int main(int argc, char const *argv[]) {
    sf_mem_init();


    int* x = sf_malloc(sizeof(int));
    sf_show_heap();

    sf_free(x);
    sf_show_heap();
    // sf_free(y);
    // sf_show_heap();


    sf_mem_fini();

    return EXIT_SUCCESS;
}
