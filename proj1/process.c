/* name: Yoodong Lee, Won Jun Cho */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "procHelp.h"

void process(char *flags, char *pid) {

	/* for reading directory*/
	char *buffer = (char *) malloc(1024 *sizeof(char));
	DIR *dir;
	struct dirent *sd;

	char *myPid;

	dir = opendir("/proc");

	if(pid != NULL){
		myPid = pid;
	}

	if(dir == NULL){
		printf("Error! Unable to open directory.\n");
		exit(1);
	}else{
		while( (sd=readdir(dir)) != NULL){   /* read each directory*/
			if(flags[0] != '0'){ /* -p option*/
				if(strcmp(sd->d_name, myPid)){
					sprintf(buffer, "/proc/%s/stat",pid);

					/* pass flags and buffer for reading the file*/
					readFile(flags, buffer, pid);
					break;
				}
			}else{
				/* no -p option */
				/* only send directory with numbers (only pid directories)*/   
				if(onlyNum(sd->d_name)){
					sprintf(buffer, "/proc/%s/stat",sd->d_name);
					int result = readUid(buffer);
					if(result == 1){
						readFile(flags, buffer, sd->d_name);
					}
				}
			}
		}
	}
	free(buffer);
}









