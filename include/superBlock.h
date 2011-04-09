#ifndef _SUPER_BLOCK_H_
#define _SUPER_BLOCK_H_

typedef struct superBlockStruct {
  uint block_size; 
  uint block_count;  // Number of blocks in the filesystem
  uint free_blocks;  // Number of free blocks in the filesystem
  uint free_inodes;  // Number of free inodes in the filesystem
  uint inode_count;  // Total number of inodes in the filesystem
} superBlock;

#endif
