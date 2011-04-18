#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"fileSystem.h"

int my_create(char *filename) {
  /**
   * Returns 0 in case of successful creation 
   * returns -1 in case of error (out of free inodes)
   * returns -2 in case the file already exists
   */
  int ind,i,index;
  if(my_open(filename,'b')>=0)
    return -2;
  if(!file_system->super_block.free_inodes)
    return -1;	// Error: No free inode
  For(ind,file_system->super_block.inode_count) {
    if(file_system->free_inode[ind].inode_bitmap==true) {
      file_system->free_inode[ind].inode_bitmap=false;
      file_system->super_block.free_inodes--;
      file_system->inode_list[ind].filename=(char*)calloc(MAX_PATH,sizeof(char));
      strcpy(file_system->inode_list[ind].filename,filename);
      file_system->inode_list[ind].filetype=NORMAL;
      file_system->inode_list[ind].filesize=0;
      For(i,MAX_LINKS)
	file_system->inode_list[ind].links[i]=-1;
      break;
    }
  }
  index=file_system->inode_list[current_inode].filesize++;
  file_system->inode_list[current_inode].links[index]=ind;
  return 0;
}

int my_open(char *filename,char mode) {
  /**
   * Returns the pointer to the inode of corresponding file, if found.
   * If not found, returns null (-1).
   */
  int i,ind;
  for(i=2;i<file_system->inode_list[current_inode].filesize;i++) {
    ind=file_system->inode_list[current_inode].links[i];
    if(!strcmp(filename,file_system->inode_list[ind].filename)&&file_system->inode_list[ind].filetype==NORMAL)
      return ind;
  }  
  return -1; // Error: file not found
}

char *my_read(char *filename,int posn,int nbytes) {
  /**
   * my_read with positive arguments returns the selective buffer of selected file
   * if file not found or selected size is more than filesize, error is thrown
   * in case of no argument in read command, the full file is read -- the functionality of "cat"
   */
  int fd=my_open(filename,'r');
  if(fd<0)
    return "foo"; // Error: file not found
  inode *file=&(file_system->inode_list[fd]);
  if(posn==-1&&nbytes==-1) {
    posn=0;
    nbytes=file->filesize;
  }
  else if(posn+nbytes>file->filesize)
    return NULL;  // Error: attempt to read more than filesize, segmentation fault.
  char *data=(char*)calloc(nbytes,sizeof(char));
  int i;
  For(i,nbytes) {
    int link_ind=(posn+i)/file_system->super_block.block_size;
    int link_pos=(posn+i)%file_system->super_block.block_size;
    data[i]=file_system->data_blocks[file->links[link_ind]].data[link_pos];
  }
  return data;
}

int my_write(char *filename,char *data) {
  /**
   * The given filename is opened through my_open
   * if not found, null is returned
   * else data is appended to the file.
   */
  int fd=my_open(filename,'w');
  if(fd<0)
    return -1;  // Error: file not found
  inode *file=&(file_system->inode_list[fd]);
  int n=strlen(data),i,ind;
  For(i,n) {
    int link_ind=file->filesize/file_system->super_block.block_size;
    int link_pos=file->filesize%file_system->super_block.block_size;
    if(!link_pos) {
      if(!file_system->super_block.free_blocks)
	return -2; // Error: No free blocks
      For(ind,file_system->super_block.block_count) {
	if(file_system->free_block[ind].block_bitmap==true) {
	  file->links[link_ind]=ind;
	  file_system->free_block[ind].block_bitmap=false;
	  file_system->super_block.free_blocks--;
	  break;
	}
      }
    }
    // One byte of data is written to the next free section
    file_system->data_blocks[file->links[link_ind]].data[link_pos]=data[i];
    file->filesize++;
  }
  return 0;
}

int my_cat(char *filename) {
  char *data=my_read(filename,-1,-1);
  if(!strcmp(data,"foo"))
    printf("cat: %s: file not found\n",filename);
  else printf("%s\n",data);
  return 0;
}

int my_copyfromfs(char *source,char *dest) {
  int in=my_open(source,'r');
  if(in==-1)
    return -1;
  FILE *out=fopen(dest,"w");
  int size=file_system->inode_list[in].filesize;
  char *data=my_read(source,0,size);
  fprintf(out,"%s",data);
  fclose(out);
  return 0;
}

int my_copytofs(char *source,char *dest) {
  my_create(dest);
  int out=my_open(dest,'w');
  if(out==-1)
    return -1;
  FILE *in=fopen(source,"r");
  if(in==NULL)
    return -2;
  char *ch=(char*)calloc(2,sizeof(char));
  while((ch[0]=fgetc(in))!=EOF) {
    ch[1]='\0';
    my_write(dest,ch);
  }
  fclose(in);
  return 0;
}

int my_delete(char *filename) {
  int fd=my_open(filename,'d');
  return my_delete2(fd,current_inode);
}

int my_delete2(int fd,int parent) {
  int index,j,i;
  if(fd>=file_system->super_block.inode_count||fd<0)
    return -1;			// Error: Segmentation Fault
  inode *file=&(file_system->inode_list[fd]);
  for(i=0;i<MAX_LINKS;i++) {
    if(file->links[i]!=-1) {
      int ind=file->links[i];
      file_system->free_block[ind].block_bitmap=true;
      file->links[i]=-1;
      file_system->super_block.free_blocks++;
    }
  }
  file_system->free_inode[fd].inode_bitmap=true;
  file_system->super_block.free_inodes++;
  
  for(i=2;i<file_system->inode_list[parent].filesize;i++) {
    index=file_system->inode_list[parent].links[i];
    if(index==fd) {
      j=i;
      break;
    }
  }
  file_system->inode_list[parent].links[j]=-1;
  for(i=j+1;i<file_system->inode_list[parent].filesize;i++) {
    file_system->inode_list[parent].links[i-1] = file_system->inode_list[parent].links[i];
  }
  file_system->inode_list[parent].filesize--; 
  return 0;
}

int my_mkdir(char *dirname) {
  int ind,i,index;
  if(!file_system->super_block.free_inodes) 
    return -1;
  if(file_system->inode_list[current_inode].filesize>=MAX_LINKS)
    return -2;
  if(my_open(dirname,'r')!=-1)
    return -3;

  For(ind,file_system->super_block.inode_count) {
    if(file_system->free_inode[ind].inode_bitmap==true) {
      file_system->free_inode[ind].inode_bitmap=false;
      file_system->super_block.free_inodes--;
      
      file_system->inode_list[ind].filename=(char*)calloc(MAX_PATH,sizeof(char));
      strcpy(file_system->inode_list[ind].filename,dirname);
      file_system->inode_list[ind].filetype=DIR;
      file_system->inode_list[ind].filesize=2;
      file_system->inode_list[ind].links[0]=ind;
      file_system->inode_list[ind].links[1]=current_inode;
      for(i=2;i<MAX_LINKS;i++)
	file_system->inode_list[ind].links[i]=-1;
      
      index=file_system->inode_list[current_inode].filesize++;
      file_system->inode_list[current_inode].links[index]=ind;
      break;
    }
  }
  return 0;
}

int my_rmdir(char * name) {
  int ind=-1,i,index,j;
  for(i=2;i<file_system->inode_list[current_inode].filesize;i++) {
    index = file_system->inode_list[current_inode].links[i];
    if(!strcmp(file_system->inode_list[index].filename,name)&&file_system->inode_list[index].filetype==DIR) {
      ind=index;
      j=i;
      break;
    }
  }
  if(ind==-1) 
    return -1; //Directory not found
  
  for(i=2;i<file_system->inode_list[ind].filesize;i++) {
    index=file_system->inode_list[ind].links[i];
    if(file_system->inode_list[index].filetype==DIR)
      my_rmdir(file_system->inode_list[index].filename);
    else if(file_system->inode_list[index].filetype==NORMAL)
      my_delete2(index,ind);
    else
      return -1; //unexpected error
  }
  file_system->inode_list[current_inode].links[j]=-1;
  for(i=j+1;i<file_system->inode_list[current_inode].filesize;i++)
    file_system->inode_list[current_inode].links[i-1]=file_system->inode_list[current_inode].links[i];
  file_system->inode_list[current_inode].filesize--;
  file_system->free_inode[ind].inode_bitmap=true;
  file_system->super_block.free_inodes++;
  return 0;
}

int my_chdir(char *name) {
  int n1=0,i,ind,found=0;
  char *name2;
  if(strcmp(name,"..")==0) {
    if(strcmp(current_path,"/")==0) {
      return -1;		//already at root
    }
    for(i=strlen(current_path)-2;i>=0;i--) {
      if(current_path[i]=='/') {
	n1=i;
	break;
      }
    }
    name2=(char *)calloc(sizeof(char),MAX_PATH);
    strncpy(name2,current_path,n1);
    name2[n1]='\0';
    current_path=name2;
    strcat(current_path,"/");
    current_inode=file_system->inode_list[current_inode].links[1];
    return 0;
  }
  else {
    found=-1;
    for(i=2;i<file_system->inode_list[current_inode].filesize;i++) {
      ind=file_system->inode_list[current_inode].links[i];
      //printf("Dir = %s\n",file_system->inode_list[ind].filename);
      if(strcmp(name,file_system->inode_list[ind].filename)==0&&file_system->inode_list[ind].filetype==DIR) {
	found=ind;
	break;
      }
    }
    if(found>=0) {
      strcat(current_path,name);
      strcat(current_path,"/");
      current_inode=ind;
      return 0;
    }
  }
  return -2;
}

int my_pwd() {
  printf("%s\n",current_path);
  return 0;
}

int my_ls() {
  int i,ind;
  for(i=2;i<file_system->inode_list[current_inode].filesize;i++) {
    ind=file_system->inode_list[current_inode].links[i];
    printf("%-20s\t%-20d\t%s\n",file_system->inode_list[ind].filename,\
	   file_system->inode_list[ind].filetype==NORMAL?file_system->inode_list[ind].filesize:file_system->inode_list[ind].filesize-2,\
	   file_system->inode_list[ind].filetype==NORMAL?"FILE":"DIR");
  }
  return 0;
}

int my_move(char *source,char *dest) {
  my_create(dest);
  int in=my_open(source,'r');
  int size=file_system->inode_list[in].filesize;
  char *data=my_read(source,0,size);
  my_write(dest,data);
  my_delete(source);
  return 0;
}
