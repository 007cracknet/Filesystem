#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

extern void create_FileSystem(int sFileSystem,int sBlock,int nInodes,int nBlocks);
extern int my_create(char *filename);
extern int my_open(char *filename,char mode);
extern int my_write(char *filename,char *data);
extern char *my_read(char *filename,int posn,int nbytes);
extern int my_cat(char *filename);
extern int my_delete(char *filename);
extern int my_delete2(int fd,int parent);
extern int my_copyfromfs(char *source,char *dest);
extern int my_copytofs(char *source,char *dest);
extern int my_mkdir(char *name);
extern int my_rmdir(char *name);
extern int my_chdir(char *name);
extern int my_pwd();
extern int my_ls();
extern int my_move(char *dest,char *source);

#endif
