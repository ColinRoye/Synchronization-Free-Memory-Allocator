#include <stdio.h>
#include <errno.h>
#include "sfmm.h"
#include "sf_listHelpers.h"

int main(int argc, char const *argv[]) {
    sf_mem_init();

    sf_malloc(10);

    sf_show_heap();


    sf_mem_fini();

    return EXIT_SUCCESS;
}
