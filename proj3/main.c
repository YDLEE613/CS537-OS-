#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "build_spec_graph.h"
#include "proc_creation_prog_exe.h"
#include "text_parsing.h"

#define MAX_DEP 64

int main(int argc, char* argv[]) {
	int c; 
	char *filename = NULL;
	char *target;
	
	// get user input option
	while((c = getopt(argc, argv, "f:")) != -1){
		switch(c){
		case 'f':
			if(optarg == NULL){
				fprintf(stderr, "Error: There should be a file name after this option\n");
				exit(1);
			}
			filename = optarg;
				
			break;
		case '?':
			fprintf(stderr, "Invalid Option (%c)", c);
			exit(1);
		}
	}
	
	// get array of TNode with parsed data
	TNode **node = parse_nodes(filename);
	
	//find duplicate target
	for(int i = 0; node[i] != NULL; i++){
		for(int j = 0; node[j] != NULL; j++){
			if(node[i] != node[j] && !strcmp(node[i]->node_name, node[j]->node_name)){
				fprintf(stderr, "Error: Duplicate target <%s> found\n", node[i]->node_name);
				exit(1);
			}
		}
	}
	
	// if there were an input, store at target
	target = argv[optind];
	int isTarget = 0;
	TNode *target_node;
	
	// search for the same node 
	if(target!=NULL){
		target_node = searchNode(node, target);
		if(target_node != NULL){
			isTarget = 1;
		}
	}
	
	// allocate memory
   	TNode **custom_array = (TNode**)malloc(MAX_DEP * sizeof(TNode*));
	for(int i = 0;i < MAX_DEP; i++){
		custom_array[i] = NULL;
	}
	
	// make an array of TNode based on target_node
	if(isTarget){
		custom_array[0] = target_node;
		makeCustom(target_node, custom_array, node);
	}else if(target == NULL){
		target_node = node[0];
	}else{
		fprintf(stderr, "Invalid Target: %s\n", target);
		exit(1);	
	}

	// connect TNode array 
	if(!isTarget){
		connectAll(node);
	}else{
		connectAll(custom_array);
	}
	
	// build the order
	TNode **final = build(target_node);
		
	// execute all the commands
	executeAll(final);
	
	// free
	free(node);
	free(final);
	return 0;
}
//
