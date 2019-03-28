#include <stdio.h>
#include <errno.h>
#include "sfmm.h"
#include "sf_listHelpers.h"

int main(int argc, char const *argv[]) {
    sf_mem_init();

    sf_errno = 0;
    /* void *x = */ sf_malloc(8);
    void *y = sf_malloc(32);
    /* void *z = */ sf_malloc(1);

    sf_free(y);
    sf_show_heap();


    sf_mem_fini();

    return EXIT_SUCCESS;
}
