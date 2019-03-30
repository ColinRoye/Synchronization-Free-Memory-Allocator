#include <stdio.h>
#include <errno.h>
#include "sfmm.h"
#include "sf_listHelpers.h"




int main(int argc, char const *argv[]) {

    sf_mem_init();

    void* ptr = NULL;
    printf("%p",sf_realloc(ptr, 100));
    printf("%d", sf_errno);
    return EXIT_SUCCESS;
}
