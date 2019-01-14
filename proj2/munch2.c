//Won Jun Cho, Yoodong Lee
//wcho8, won || ydlee399, yoodong
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "munch2.h"
#include "queue.h"

void *toUpper(void* q){

  // cast q into Queue**
  Queue** queues = (Queue**)q;
  
  char *string = NULL;
  char *curr = NULL;
  
  // change every letter to upper case
  while(queues[1]->size != 0 || queues[1]->done != 1){
    if(NULL == (string = DequeueString(queues[1]))){
      fprintf(stderr, "ERROR: Fail to get string from dequeueString\n");
      return NULL;
    }
    curr = string;
    int i = 0;
    while(curr[i]!='\0'){
      curr[i] = toupper(curr[i]);
      i++;
    }
    
    // enqueue changed string 
    EnqueueString(queues[2], curr);
  }
  
  // signal done with the Queue
  queues[2]->done = 1;
  
  // exit thread
  pthread_exit((void*)q);
}
