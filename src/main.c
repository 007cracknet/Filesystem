#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"fileSystem.h"

int main() {
  int sFileSystem,sBlock,nInodes,nBlocks,argc,status;
  char input[MAX_LEN],*argv[MAX_ARGS],*cp,dummyc,*forW,*inputCopy;
  const char *delim=" \t\n";
  printf("Enter the filesystem size in bytes: ");
  scanf("%d",&sFileSystem);
  printf("Enter the size of each block in bytes: ");
  scanf("%d",&sBlock);
  printf("Enter the number of inodes: ");
  scanf("%d",&nInodes);
  nBlocks=sFileSystem/sBlock;
  create_FileSystem(sFileSystem,sBlock,nInodes,nBlocks);
  printf("Filesystem mounted\n");

  scanf("%c",&dummyc);
  while(1) {
    printf(">>");
    if(fgets(input,sizeof(input),stdin)==NULL) {
      printf("Ending session\n");
      exit(EXIT_SUCCESS);
    }
    inputCopy=(char*)calloc(MAX_LEN,sizeof(char));
    strcpy(inputCopy,input);
    if(!strcmp(input,"\n"))
      continue;
    cp=input;
    For(argc,MAX_ARGS) {
      if((argv[argc]=strtok(cp,delim))==NULL)
	break;
      cp=NULL;
    }
    if(!strcmp(argv[0],"exit")) {
      printf("Ending session\n");
      exit(EXIT_SUCCESS);
    }
    else if(!strcmp(argv[0],"create")) {
      if(argc==2) {
	status=my_create(argv[1]);
	if(status==-1)
	  printf("Error: No free space\n"); 
	else if(status==-2)
	  printf("Error: File already exists\n");
      }
      else printf("Usage: create FILE\n");
    }
    else if(!strcmp(argv[0],"open")) {
      if(argc==1) {
	if(my_open(argv[1],'d')==-1) //dummy paramter passed
	  printf("Error: File not found\n");
      }
      else printf("Usage: open FILE");
    }
    else if(!strcmp(argv[0],"read")) {
      char *red;
      if(argc==2)
	red=my_read(argv[1],-1,-1);
      else if(argc>4||argc<2)
	printf("Usage: read FILE [init_position] [buffersize]\n");
      else
	red=my_read(argv[1],atoi(argv[2]),atoi(argv[3]));
      if(red==NULL)
      	printf("Error: Buffer size exceeded\n");
      else if(!strcmp(red,"foo"))
	printf("Error: File not found\n");
      else printf("%s\n",red);
    }
    else if(!strcmp(argv[0],"write")) {
      if(argc<3)
	printf("Usage: write FILE <data>\n");
      else {
	forW=(char*)malloc(MAX_DATA*sizeof(char));
	int begin=-1,loop,len=strlen(inputCopy);
	For(loop,len) {
	  if(inputCopy[loop]=='"') {
	    if(begin<0)
	      begin=loop;
	    else
	      break;
	  }
	  else if(begin>=0)
	    forW[loop-begin-1]=inputCopy[loop];
	}
	int status=my_write(argv[1],forW);
	if(status==-1)
	  printf("Error: File not found\n");
	else if(status==-2)
	  printf("Error: No free space\n");
      }
    }
    else if(!strcmp(argv[0],"cat")) {
      if(argc!=2)
	printf("Usage: cat FILE\n");
      else my_cat(argv[1]);
    }
    else if(!strcmp(argv[0],"cpffs")) {
      if(argc!=3)
	printf("Usage: cpffs SOURCE DEST\n");
      else my_copyfromfs(argv[1],argv[2]);
    }
    else if(!strcmp(argv[0],"cptfs")) {
      if(argc!=3)
	printf("Usage: cptfs SOURCE DEST\n");
      else my_copytofs(argv[1],argv[2]);
    }
    else if(!strcmp(argv[0],"delete")) {
      if(argc!=2)
	printf("Usage: delete FILE\n");
      else my_delete(argv[1]);
    }
    else if(!strcmp(argv[0],"mkdir")) {
      my_mkdir(argv[1]);
    }
    else if(!strcmp(argv[0],"rmdir")||!strcmp(argv[0],"rm")) {
      my_rmdir(argv[1]);
    }
    else if(!strcmp(argv[0],"chdir")||!strcmp(argv[0],"cd")) {
      if(argc!=2) printf("Usage: cd DIR\n");
      else if(my_chdir(argv[1])==-1)
	printf("%s : already at root\n",argv[0]);;
      }
    else if(!strcmp(argv[0],"pwd")) {
      my_pwd();
    }
    else if(!strcmp(argv[0],"ls")) {
      my_ls();
      }
    else if(!strcmp(argv[0],"mv")) {
      if(argc==3) {
	my_move(argv[1],argv[2]);
      }
      else printf("Usage: mv SOURCE DEST\n");
    }
    else printf("Invalid command\n");
  }
  return 0;
}
