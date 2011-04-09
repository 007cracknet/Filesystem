CC= gcc
CFLAGS= -Wall -g -c
INCLUDES= -I ./include/
OBJ= ./obj/fileSystem.o ./obj/functions.o ./obj/main.o

all: fs
fs: $(OBJ)
	$(CC) -g $(INCLUDES) -o fs $(OBJ)
./obj/fileSystem.o: ./src/fileSystem.c
	$(CC) $(CFLAGS) $(INCLUDES) -o ./obj/fileSystem.o ./src/fileSystem.c
./obj/functions.o: ./src/functions.c
	$(CC) $(CFLAGS) $(INCLUDES) -o ./obj/functions.o ./src/functions.c
./obj/main.o: ./src/main.c
	$(CC) $(CFLAGS) $(INCLUDES) -o ./obj/main.o ./src/main.c
clean:
	rm -rf $(OBJ)
	rm -rf ./fs
	rm -rf ./src/*~ ./*~ ./include/*~
.PHONY: clean