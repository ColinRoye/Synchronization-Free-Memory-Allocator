
#ifndef SF_LF_H
#define SF_LF_H
#include "sfmm.h"
unsigned int getNextAlloc(sf_block* ptr);
unsigned int getPrevAlloc(sf_block* ptr);
unsigned int getBlockSize(sf_block* ptr);
unsigned int getRequestedSize(sf_block* ptr);
sf_block *getNext(sf_block* ptr);
sf_block *getPrev(sf_block* ptr);
sf_block *getNextInMem(sf_block* ptr);
sf_block *getPrevInMem(sf_block* ptr);
void setPrevAlloc(sf_block* ptr, unsigned int prevAlloc);
void setNextAlloc(sf_block* ptr, unsigned int nextAlloc);
int setBlockSize(sf_block* ptr, unsigned int block_size);
void setRequestedSize(sf_block* ptr, unsigned int requested_size);
void setPrev(sf_block* ptr, sf_block* prev);
void setNext(sf_block* ptr, sf_block* next);
sf_block* splitBlock(sf_block* ptr, unsigned int block_size, unsigned int requested_size);
void setFooter(sf_block* ptr, unsigned int block_size, unsigned int prevAlloc, unsigned int nxtAlloc);
void setAllocHeader(sf_block* ptr, unsigned int block_size, unsigned int prevAlloc, unsigned int nextAlloc, unsigned int requested_size);
void setFreeHeader(sf_block* ptr, unsigned int block_size, unsigned int prevAlloc, unsigned int nextAlloc, sf_block prev, sf_block next);
void clearHeader(sf_block* ptr);
void initPrologue(sf_block* ptr);
void initEpilogue(sf_block* ptr);
void initFreeBlock(sf_block* ptr, sf_block prev, sf_block next, unsigned int block_size, unsigned prevAlloc, unsigned nextAlloc);
int initFirstBlock();
void clearBlock(sf_block* ptr);
unsigned int coaless(sf_block* ptr);
unsigned int addPage();
#endif
