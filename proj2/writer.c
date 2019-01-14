//Won Jun Cho, Yoodong Lee
//wcho8, won || ydlee399, yoodong
#include <stdlib.h>
#include <stdio.h>
#include "writer.h"
#include "queue.h"

void* writeOut(void* q){

  // cast q into Queue**
  Queue** queues = (Queue**)q;
  
  char *string = NULL; // output
  int count = 0; // counter for number of string
  
  // print strings in the Queue
  while(queues[2]->size != 0 || queues[2]->done != 1){
    if(NULL == (string = DequeueString(queues[2]))){
      fprintf(stderr, "ERROR: Fail to get string from dequeueString.\n");
    } 
    
    count++;
    printf("Output: %s\n", string);
    free(string);
    sem_post((sem_t*)queues[3]);
  }
  
  
  
  // print stats for every queue
  printf("---------------------------\n");
  printf("Number of String: %d\n", count);
  printf("---------------------------\n");
  int i;
  for( i=0; i<3; i++){
    if(i==0){
      printf("From Reader to Munch1:\n");
    }else if(i==1){
      printf("From Munch1 to Munch2:\n");
    }else{
      printf("From Munch2 to Writer:\n");
    }
    PrintQueueStats(queues[i]);
    printf("---------------------------\n");
  }
  
  // exit thread
  pthread_exit((void*)q);
} 
