/* name: Yoodong Lee, Won Jun Cho */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "process.h"

int main(int argc, char *argv[])
{

	/* for command*/
	int c;
	char *pid = NULL; 
	int err = 0;

	/* default: U and c */
	char flags[6] = {'0','0','U','0','0','c'}; 
	char rp[6] = {0, 0, 0, 0, 0, 0};

	/* parse command lines and change flags*/
	while((c=getopt(argc, argv, "p:s::U::S::v::c::")) != -1){

		switch(c){
		case 'p':
			flags[0] = 'p';
			pid = optarg;
			break;
		case 's':
			if(rp[1] == 1){ 
				err = -1;
			}
			if(optarg == NULL){
				flags[1] = 's';
				rp[1] = 1;
			}else if(strcmp(optarg, "-") == 0){
				rp[1] = 1;
			}else{
				err = -1;
			}
			break;
		case 'U':
			if(rp[2] == 1){
				 err = -1;
			}
			if(optarg == NULL){
				rp[2] = 1;
			}else if(strcmp(optarg, "-") == 0){
				flags[2] = '0';
				rp[2] = 1;
			}else{
				err = -1;
			}
			break;
		case 'S':
			if(rp[3] == 1){
				 err = -1;
			}
			if(optarg == NULL){
				flags[3] = 'S';
				rp[3] = 1;
			}else if(strcmp(optarg, "-") == 0){
				rp[3] = 1;
			}else{
				err = -1;
			}
			break;
		case 'v':
			if(rp[4] == 1){
				 err = -1;
			}
			if(optarg == NULL){
				flags[4] = 'v';
				rp[4] = 1;
			}else if(strcmp(optarg, "-") == 0){
				rp[4] = 1;
			}else{
				err = -1;
			}
			break;
		case 'c':
			if(rp[5] == 1){
				 err = -1;
			}
			if(optarg == NULL){
				rp[5] = 1;
			}else if(strcmp(optarg, "-")==0){
				flags[5] = '0';
				rp[5] = 1;
			}else{
				err = -1;
			}
			break;
		default:
			exit(1);
		}
	}


	if (optind < argc || err == -1 ) {
		printf("Error: invalid option\n");
		exit(1);
	}

	process(flags, pid);

	return 0;
}
