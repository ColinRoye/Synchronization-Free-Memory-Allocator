#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "sfmm.h"
#include "sf_listHelpers.h"
#include "sfmm.h"

sf_block* initEmptyBlock(){
    sf_block* empty = malloc(sizeof(sf_block)) ;
    empty->header.block_size = 0;
    empty->header.requested_size = 0;
    empty->body.links.next = NULL;
    empty->body.links.prev = NULL;
    return empty;
}