#ifndef BUILD_SPEC_REPR
#define BUILD_SPEC_REPR 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Node{
  struct Node *parent; // target
  struct Node **child; // dependencies
   
  
  char **commands;
  char *node_name; // self
  
  int checked; 
  int level;
  int child_count;
  int command_count;
  int order;
  int again;

 
} TNode;

// initialize node
TNode* initNode(char *, int);

// add child as a node
void addChild(TNode *, TNode *);

// add command
void addCommand(TNode*, char*);

// search Node
TNode* searchNode(TNode**,char*);

// post-order traversal
void post_order(TNode *, int); 

// free Node
void freeNode(TNode*);

// print basic info of a node
void printInfo(TNode*);
#endif
