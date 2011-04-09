#ifndef _FREE_BLOCK_H_
#define _FREE_BLOCK_H_

typedef struct freeBlockStruct {
  enum bool block_bitmap; // true:free; false:occupied;
} freeBlock;

#endif
