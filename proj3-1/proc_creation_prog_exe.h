#ifndef PROC_CREATION_PROG_EXE
#define PROC_CREATION_PROG_EXE
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "build_spec_repr.h"

// parse path
char *getPath(TNode *);

// execute command
int executeCommands(char **);

// execute commands in all the TNode in array
void executeAll(TNode**);

// parse command
char **makeCommand(char*);

// compare modification time
int compModtime(char*,char*);
#endif