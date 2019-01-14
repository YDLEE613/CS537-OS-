/* name: Yoodong Lee, Won Jun Cho */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "procHelp.h"

#define SNUMOPT 3

/*
  flags: options from command line
  buffer: directory to read 

*/
void readFile(char *flags, char *buffer, char *pid){
  /* for reading file content*/
  FILE *file;
  char *state = (char *) malloc(256 * sizeof(char));
  char *utime = (char *) malloc(256 * sizeof(char));
  char *stime = (char *) malloc(256 * sizeof(char));
  int sizeM = 0;
  char *cmdLine = (char *) malloc(256 * sizeof(char));
  char *result = (char *)malloc(256 *sizeof(char));
  char *tmp = (char *)malloc(64 * sizeof(char));
  
  /* pid is printed for any case */
  sprintf(result, "%s: ", pid); 
  
    /*
    From stat file
     #3:  state  -s
     #14: utime  -U  default:true
     #15: stime  -S
    */
  file = fopen(buffer, "r"); /*buffer: /proc/<pid>/stat*/
      
  if(file == NULL)
  {
    /* either error or not a process directory*/
    printf("Error: cannot open file\n");
    exit(1);
  }else
  {
      /*first read stat file and store numbers*/
    char **statResult = parseStat(file);
    
    if(flags[1] == 's'){  /* -s flag */
     /* #3 */
     state = statResult[0];
     
     /* concat state to the reuslt */
     sprintf(tmp, "%s", state);
     strcat(result, state); 
    }
    
    if(flags[2] == 'U'){  /* -U flag */
      /* #14 */
      utime = statResult[1];
      
      /* concat utime to the result */
      sprintf(tmp, " utime=%s", utime);
      strcat(result, tmp);
      
    }
    
    if(flags[3] == 'S'){  /* -S flag */
      /* #15 */ 
      stime = statResult[2];
      
      /* concat stime to the result */
      sprintf(tmp, " stime=%s", stime);
      strcat(result, tmp);
    }
  }

  /* 
    From statm file
     #1: size -v
  */
  if(flags[4] == 'v'){
    sizeM = parseStatm(file, buffer);
    
    /* concat stime to the result */
    sprintf(tmp, " size=%d", sizeM);
    strcat(result, tmp);
  }else{
    sprintf(buffer, "%s%s", buffer, "m");
  }
  
  /*
    From cmdline
      -c   default: true;
  */
  if(flags[5] == 'c'){
    cmdLine = readCmd(file, buffer);
    
    /* concat stime to the result */
    sprintf(tmp, " [%s]", cmdLine);
    strcat(result, tmp);
  }
    
  printf("%s\n", result);
  free(cmdLine);
  free(tmp); 
}

/*
  *file: pointer to the file that needs to be read
*/
char **parseStat(FILE *file){
  char *buffer = (char *) malloc(1024 * sizeof(char));
  
  /*list of strings to store state, utime, stime*/
  int i = 0;
  char **retVal = malloc(3 *sizeof(char*));
  for(i = 0; i<SNUMOPT; i++){
    retVal[i] = malloc(256);
    
  }
   
  fread(buffer, 1024,1,file);
  
  /* tokenize buffer by a single space*/
  int count = 0;
  int index = 0;
  char delim[] = " ";
  char *tokenBf = strtok(buffer, delim);
  
  
  while(tokenBf != NULL){
    count++;
    if(count == 3){
      retVal[index] = tokenBf;
      index++;
    }
    if(count == 14){
      retVal[index] = tokenBf;
      index++;
    }
    if(count == 15){
      retVal[index] = tokenBf;
      index++;
    }
    tokenBf = strtok(NULL, delim);
  }
  free(tokenBf);
  
  return retVal;
}


/*
  *file: pointer to the file that needs to be read 
  *buffer: string that holds directory
*/
int parseStatm(FILE *file, char *buffer){
 
  /* -v flag*/
  char *m = "m";
  strcat(buffer, m); /* directory for statm */
  char *newBf = (char *) malloc(256 *sizeof(char));
  
  file = fopen(buffer, "r"); /* read statm again*/
  
  if(file == NULL){
    /* either error or not a process directory*/
    printf("Error: cannot open file\n");
    exit(1);
  }else{
    fread(newBf, 256, 1, file);   
  }
  /* tokenize buffer */
  char delim[] = " ";
  char *tokenBf = strtok(newBf, delim);
  
  return atoi(tokenBf);
}


/*
  *file: pointer to the file that needs to be read
  *buffer: string that holds directory
*/
char *readCmd(FILE *file, char *buffer){
  char *ret = (char *) malloc(1024 * sizeof(char));
  char *newBf = (char *) malloc(256 * sizeof(char));
  char *cmd = "cmdline";
  
  /* change directory to read cmdline file in the same directory*/
  memcpy(newBf, buffer, strlen(buffer)-5);
  strcat(newBf, cmd);
  
  file = fopen(newBf, "r");
  
  if( file == NULL){
    /* either error or not a process directory*/
    printf("Error: cannot open file\n");
    exit(1);
  }else{
    fread(ret, 1024, 1, file);
  } 
 
  
  return ret; 
}


/*
  read uid from each process, check if it has the same uid,
  and return 1 if same, otherwise 0.
  
  *buffer: directory to read
  
*/
int readUid(char *directory){

  char c[256];
  char *newBf = (char *) malloc(256 * sizeof(char)); /* new buffer */
  char *st = "status";
  FILE *file;
  
  int myUid = getuid(); /* my uid*/
  
  /* change directory to read status file */
  memcpy(newBf, directory, strlen(directory)-4);
  
  strcat(newBf, st);
  
  file = fopen(newBf, "r");
  
  if(file == NULL){
    printf("Error: cannot open file\n");
    exit(1);
  }else{
    int i = 0;
    while(fgets(c, 256, file)){
      i++;
      char delim[] = "\t";
      
      /* tokenize each line to get uid of the process*/
      if( i == 9){ /* uid of process*/
        int count = 0;
        char *psUid = strtok(c, delim);
        
        while(psUid != NULL){
          count++;
          psUid = strtok(NULL, delim);
          if(count == 1){
            if(myUid == atoi(psUid)){
              
              return 1;
            }else{
              
              return 0;
            }
          }
        }
      }
    }
  }
  return 0;
}

int onlyNum(char *buffer){
  while(*buffer){
    if(!isdigit(*buffer++)){
      return 0;
    }
  }
  return 1;
}

