#ifndef _FREE_INODE_H_
#define _FREE_INODE_H_

typedef struct freeInodeStruct {
  enum bool inode_bitmap; // true:free; false:occupied;
}freeInode;

#endif
