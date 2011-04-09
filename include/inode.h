#ifndef _INODE_H_
#define _INODE_H_

typedef struct inodeStruct {
  char *filename;
  uint filetype;        // 1:FILE; 2:DIRECTORY;
  uint filesize;        // number of bytes (For file, no. of bytes occupied, for directory, no. of files existing)
  int links[MAX_LINKS]; //pointer to block for file, pointer to inode for directory
} inode;

#endif
