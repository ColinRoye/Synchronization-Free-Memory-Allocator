#include <stdio.h>
#include <errno.h>
#include "sfmm.h"
#include "sf_listHelpers.h"

int main(int argc, char const *argv[]) {
    sf_mem_init();

    sf_errno = 0;
    /* void *w = */ sf_malloc(8);
    sf_show_heap();
    void *x = sf_malloc(200);
    sf_show_heap();
    void *y = sf_malloc(300);
    sf_show_heap();
    /* void *z = */ sf_malloc(4);
    sf_show_heap();
    sf_free(y);
    sf_show_heap();
    sf_free(x);
    sf_show_heap();


    sf_mem_fini();

    return EXIT_SUCCESS;
}
