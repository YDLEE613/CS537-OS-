#ifndef BUILD_SPEC_GRAPH
#define BUILD_SPEC_GRAPH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "build_spec_repr.h"

// check for cycle
void isCycle(TNode *);

// make array for custom 537make
void makeCustom(TNode *, TNode**, TNode**);

// connect all node
void connectAll(TNode **);

// order the node 
TNode **build(TNode*);

// traverse 
void traverse(TNode**, TNode*);
#endif
