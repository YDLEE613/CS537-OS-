#ifndef TEXT_PARSING
#define TEXT_PARSING
#include <string.h> 
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include "build_spec_repr.h"

// parse file
TNode **parse_nodes(char*);

// pare target, dependencies, commands
TNode **parse_lines(FILE*);

// remove extra " "
char *remove_space(char*);
#endif