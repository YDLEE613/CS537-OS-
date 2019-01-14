#include "proc_creation_prog_exe.h"
#include "build_spec_repr.h"

#define MAX_COMM 64

/*
Parse path to be ./node->node_name

Input:   a TNode
Return:  path    
*/
char *getPath(TNode *node){
	char dir[3] = "./";
	char *path;

	path = (char*)malloc(sizeof(dir)+ sizeof(node->node_name));
	path[0] = '\0';
	strcat(path, dir);
	strcat(path, node->node_name);

	return path;
}

/*
Execute input commands

Input:  command
Return -1 if error. Otherwise 0.
*/
int executeCommands(char **command){
	pid_t pid;
	int status;
	
	pid = fork();
	if(pid == 0){
		execvp(command[0], command);
	}else{
		
		wait(&status);
		
		// check status
		if(WIFEXITED(status)){
			
			// error exit from child. If child exits with an error, exit
			if(WEXITSTATUS(status)){
				fprintf(stderr,"Error: Fail to run command: ");
				for(int i =0; command[i]!=NULL; i++){
					printf("%s ", command[i]);
				}
				printf("\n");
				exit(1);
			}
		}
	}
	
	return 0;
}

/*
Execute all the commands of each TNode in an array

Input:  array of TNode
*/
void executeAll(TNode **nodes){
	char *p_path, *c_path;
	int line = 0;

	// loop through the array and compare modification time
	for(int i = 0; nodes[i] != NULL; i++){
		//sleep(1);
		int comp = 0;
		p_path = getPath(nodes[i]);
		if(nodes[i]->child_count == 0){
			comp = 1;
		}
		for(int j = 0; nodes[i]->child[j] != NULL; j++){
			c_path = getPath(nodes[i]->child[j]);
			if(p_path != NULL && c_path != NULL){
				comp = compModtime(p_path, c_path);
				if(comp){
					break;
				}
			}
			free(c_path);
		}
		free(p_path);
		int size = nodes[i]->command_count;
		if(comp){
			if(!size){
				continue;
			}else{
				for(int j = 0; nodes[i]->commands[j] != NULL; j++){
					//char *command = (char*)malloc(strlen(nodes[i]->commands[j]) +1*sizeof(char));
					//strcpy(command, nodes[i]->commands[j]);
					
					char **com = makeCommand(nodes[i]->commands[j]);
					int err = executeCommands(com);
					
					line = j + nodes[i]->level + 1;
					if(err == -1){
						fprintf(stderr, "Line %d: Invalid Line \n", line);
						exit(-1);
					}
					//free(command);
				}
			}
		}else{
			continue;
		}
	}
}

/*
Parse commands to be in appropriate formate for execvp

Input:  command
Return: tokenized command
*/
char** makeCommand(char *command){
	int size = 0;
	char **comm = (char**)malloc(MAX_COMM * sizeof(char*));
	char **result;
	for(int i =0; i < MAX_COMM; i++){
		comm[i] = NULL;
	}

	// tokenize
	char *token = strtok(command, " ");
	while(token != NULL){
		comm[size] = token;
		size ++;
		token = strtok(NULL, " ");
	}
	if(size == 0){
		fprintf(stderr, "Error: Empty command detected\n");
		exit(1);
	}
	result = (char**)malloc((size+1)*sizeof(char*));
	for(int i =0; i < size; i++){
		result[i] = comm[i];
		printf("%s ", result[i]);
	}
	printf("\n");

	return result;
}

/*
Compare modification time of parent and child

Input:  names of parent and child
Return: 1 if child is newer. Otherwise 0  
*/
int compModtime(char* parent, char* child){
	struct stat p_info, c_info;
	time_t p_time = 0, c_time = 0;
	if(stat(parent, &p_info) == 0){
		p_time = p_info.st_mtime;
	}
	if(stat(child, &c_info) == 0){
		c_time = c_info.st_mtime;
	}
	if(difftime(p_time, c_time) < 0){
		return 1;
	}else{
		return 0;
	}
}
