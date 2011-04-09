#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"fileSystem.h"

void create_FileSystem(int sFileSystem,int sBlock,int nInodes,int nBlocks) {
  int i;
  file_system=(fileSystem*)calloc(1,sizeof(fileSystem));
  file_system->free_inode=(freeInode*)calloc(nInodes,sizeof(freeInode));
  file_system->free_block=(freeBlock*)calloc(nBlocks,sizeof(freeBlock));
  file_system->inode_list=(inode*)calloc(nInodes,sizeof(inode));
  file_system->data_blocks=(block*)calloc(nBlocks,sizeof(block));

  file_system->super_block.block_size=sBlock;
  file_system->super_block.block_count=nBlocks;
  file_system->super_block.free_blocks=nBlocks;
  file_system->super_block.inode_count=nInodes;
  file_system->super_block.free_inodes=nInodes-1;
  
  For(i,nBlocks) {
    file_system->data_blocks[i].data=(char*)calloc(sBlock,sizeof(char));
    file_system->free_block[i].block_bitmap=true;
  }
  for(i=1;i<nInodes;i++)
    file_system->free_inode[i].inode_bitmap=true;
  
  current_path=(char*)calloc(MAX_PATH,sizeof(char));
  strcpy(current_path,"/");
  current_inode=0;
  
  file_system->inode_list[0].filename="/";
  file_system->inode_list[0].filetype=DIR;
  file_system->inode_list[0].filesize=2;
  file_system->inode_list[0].links[0]=0;  // link to itself
  file_system->inode_list[0].links[1]=-1; // No parent present
  for(i=2;i<MAX_LINKS;i++)
    file_system->inode_list[0].links[i]=-1;
  /*This function ensures complete formatting and creation of a new filesystem with the specified size and inode count*/
}
