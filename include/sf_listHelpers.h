
#ifndef SF_LF_H
#define SF_LF_H
#include "sfmm.h"
unsigned int getNextAlloc(sf_block* ptr);
unsigned int getAlloc(sf_block* ptr);
unsigned int getPrevAlloc(sf_block* ptr);
unsigned int getBlockSize(sf_block* ptr);
unsigned int getRequestedSize(sf_block* ptr);
sf_block *getNext(sf_block* ptr);
sf_block *getPrev(sf_block* ptr);
sf_block *getNextInMem(sf_block* ptr);
sf_block *getPrevInMem(sf_block* ptr);
void setPrevAlloc(sf_block* ptr, unsigned int prevAlloc);
void setAlloc(sf_block* ptr, unsigned int nextAlloc);
int setBlockSize(sf_block* ptr, unsigned int block_size);
void setRequestedSize(sf_block* ptr, unsigned int requested_size);
void setPrev(sf_block* ptr, sf_block* prev);
void setNext(sf_block* ptr, sf_block* next);
sf_block* splitBlock(sf_block* ptr, unsigned int block_size, unsigned int requested_size, unsigned int total_size);
int setFooter(sf_block* ptr, unsigned int block_size, unsigned int prevAlloc, unsigned int nxtAlloc);
int setAllocHeader(sf_block* ptr, unsigned int block_size, unsigned int prevAlloc, unsigned int nextAlloc, unsigned int requested_size);
int setFreeHeader(sf_block* ptr, unsigned int block_size, unsigned int prevAlloc, unsigned int nextAlloc, sf_block* prev, sf_block* next);
void clearHeader(sf_block* ptr);
void initPrologue(sf_block* ptr);
void initEpilogue(sf_block* ptr);
void initFreeBlock(sf_block* ptr, sf_block* prev, sf_block* next, unsigned int block_size, unsigned prevAlloc);
int initFirstBlock();
void clearBlock(sf_block* ptr);
unsigned int coaless(sf_block* ptr);
unsigned int addPage();
void initQuickLists();
void printBlockSize(void* ptr);
int getListBlockSize(int i);
void emptyQL(int i);
void setNextQL(sf_block* ptr, int i);
int addToQuickList(sf_block* ptr);
void setNextInMemPA(sf_block* ptr, unsigned int prevAlloc);
void FL_add(sf_block* ptr);
void FL_remove(sf_block* ptr);
#endif

