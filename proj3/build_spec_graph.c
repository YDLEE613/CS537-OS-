#include "build_spec_graph.h"
#include <unistd.h>

#define MAX_DEP 64

/*
Find cycle by searching from the root node

Input:	root node
*/
void isCycle(TNode *node){
	node->checked = 1;
	node->again = 1;
	
	// loop through child of input node
	for(int i = 0; i<node->child_count; i++){
		
		// if child is not checked, use recursion
		if(node->child[i]->checked == 0){
			isCycle(node->child[i]);
		}else if(node->child[i]->again){ // if child was visited before, it is cycle
			fprintf(stderr, "Error: Cycle detected in %s\n", node->node_name);
			exit(1);
		}
	}	
}
/*
Recursively make a TNode array based on command line target

Input:	target, array to store nodes in new order, array without order 
*/
void makeCustom(TNode *node, TNode **nodes, TNode **origin){
	int cnt =0;
	for(int i =0; nodes[i] != NULL; i++){
		cnt++;
	}
	
	// check if there is cycle for each node
	isCycle(node);
	
	// loop through child and add to new array
	for(int i =0; node->child[i] != NULL; i++){
		TNode *tmp = searchNode(origin, node->child[i]->node_name);
		nodes[cnt] = tmp;
		if(tmp != NULL){
			makeCustom(tmp, nodes, origin);
		}
		int t = 0;
		for(int j = 0; nodes[j] != NULL; j++){
			t++;
		}
		if(cnt != t){
			cnt = t;
		}
	}
}

/*
Connect all TNode in the array

Input:	array of TNode
*/
void connectAll(TNode **nodes){
	int cnt = 0;
	for(int i =0; nodes[i] != NULL; i++){
		cnt++;
	}

	// loop through nodes array
	for(int i =0; i < cnt; i++){
		TNode **children = nodes[i]->child;
		
		// loop through child of each node
		for(int j = 0; j < nodes[i]->child_count; j++){
			TNode *child = searchNode(nodes, children[j]->node_name);
			
			// connect child and parent
			if(child != NULL){
				child->parent = nodes[i];
				nodes[i]->child[j] = child;
			}
		}
	}
}
 
/*
Return ordered array

Input:	root node
Return:	array of TNode
*/
TNode **build(TNode *root){
	TNode **final = malloc(MAX_DEP*sizeof(TNode*));
	for(int i =0; i < MAX_DEP; i++){
		final[i] = NULL;
	}
	 
	traverse(final, root);

	return final;
}

/*
Traverse all nodes

Input:	array of TNode, TNode
*/
void traverse(TNode **nodes, TNode *node){
	if(node != NULL){
		node->checked++;
	
		for(int i = 0; i < node->child_count; i++){
			if(node->child[i]->checked > 1){
				fprintf(stderr, "Error: Cycle detected in %s\n", node->node_name);
				exit(1);
			}
			if(node->child[i]->order != 0){
				continue;
			} 
			if(node->child[i]->level != 0){
				traverse(nodes, node->child[i]);
			}
		}
		int cnt = 0;
		while(nodes[cnt] != NULL){
			cnt++;
		}
		node->order = cnt;
		nodes[cnt] = node;
		node->checked--;
	}
}


