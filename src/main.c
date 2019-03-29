#include <stdio.h>
#include <errno.h>
#include "sfmm.h"
#include "sf_listHelpers.h"




int main(int argc, char const *argv[]) {
    sf_mem_init();

    void* a = sf_malloc(8);
    void* b = sf_malloc(8);
    void* c = sf_malloc(8);
    void* d = sf_malloc(8);
    void* e = sf_malloc(8);
    void* f = sf_malloc(8);

    sf_free(a);
    sf_free(b);
    sf_free(c);
    sf_free(d);
    sf_free(e);


    sf_free(f);
    sf_mem_fini();

    return EXIT_SUCCESS;
}
