#include "build_spec_repr.h"

#define MAX_BUFFER 1024
#define MAX_COMM 64
#define MAX_DEP 64 

/* Create node */
TNode* initNode(char *name, int level){ 
  TNode *node = (TNode *)calloc(1, sizeof(TNode));// return Node
  
  //memory allocation for children
  node->child = (TNode **)malloc(MAX_DEP * sizeof(TNode*));
  for(int i = 0; i<MAX_DEP; i++){
    node->child[i] = NULL;
  }
 
  //memory allocation for commands
  node->commands = (char**)malloc(MAX_COMM * sizeof(char*));
  for(int i = 0; i<MAX_COMM; i++){
    (node->commands)[i] = NULL;
  }
  
  // initialize variables
  node->node_name = name;
  node->child_count = 0;
  node->level = level; //line_num
  node->checked = 0;
  node->order = 0;
  node->again = 0;
  node->command_count =0;
	   
  return node;
}

/* Update: add child */
void addChild(TNode *node, TNode *child){
  node->child[node->child_count] = child;
  ++node->child_count;
}

/* Update: add commands */
void addCommand(TNode *node, char *command){
	node->commands[node->command_count] = command;
	++node->command_count;
}

// Search node
TNode *searchNode(TNode **nodes, char* name){
	int cmp;
	for(int i = 0; nodes[i] != NULL; i++){
	
		cmp = strcmp(name, nodes[i]->node_name);
		if(cmp == 0){
			return nodes[i];
		}
	}
	return NULL;
}

// post-order traversal
void post_order(TNode *node, int zero){
  if(node == NULL){
    return;
  }
  for(int i = 0; i<node->child_count; i++){
    post_order(node->child[i], zero);
  }
  
  //add node to array 
  node->order = zero;
  zero++;
}

// free node function
void freeNode(TNode *node){

	free(node->child);
	node->child = NULL;

	if(node->commands!=NULL){
		for(int i = 0; i<node->command_count; i++){
			if(node->commands[i]!=NULL){
				free(node->commands[i]);
				node->commands[i] = NULL;
			}
		}
			free(node->commands);
	
	}
	node->commands = NULL;
	free(node->node_name);
	node->node_name = NULL;
	free(node);	
	node = NULL;	
}
// print node basic info
void printInfo(TNode *node){
	printf("Line %d: %s\n", node->level, node->node_name);
	printf("Child Count: %d\n", node->child_count);
  for(int i = 0; i<node->child_count; i++){
    printf("Child : %s\n", node->child[i]->node_name);
  }
	if(node->parent != NULL){
		printf(">>> Parent: %s\n", node->parent->node_name);
	}
	for(int i = 0; i < node->command_count; i++){
		printf("\\_command: %s\n\n", node->commands[i]);
	}
	
}

