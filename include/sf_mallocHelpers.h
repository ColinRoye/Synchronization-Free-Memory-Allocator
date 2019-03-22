
#ifndef SF_MH
#define SF_MH

int actualSize(unsigned int size);
sf_block* getFreeBlock(unsigned int requested_size);
sf_block* getQuickBlock(unsigned int size);

#endif