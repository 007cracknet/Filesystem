#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include"misc.h"
#include"functions.h"
#include"superBlock.h"
#include"freeInode.h"
#include"freeBlock.h"
#include"inode.h"
#include"block.h"

typedef struct fileSystemStruct {
  superBlock super_block;
  freeInode *free_inode;
  freeBlock *free_block;
  inode *inode_list;
  block *data_blocks;
} fileSystem;

fileSystem *file_system;
char *current_path;
int current_inode;

#endif
