//Won Jun Cho, Yoodong Lee
//wcho8, won || ydlee399, yoodong
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "queue.h"

#define MAXSIZE 10
#define MAXCHAR 1024

/*
  Dynamically allocate a new Queue structure and initialize it with an array of character points of length size.   The function returns a pointer to the new queue structure.
*/

Queue *CreateStringQueue(int size){
  int i;
  Queue *q = NULL;
  // check valid parameter
  if(size>0){
    q = (Queue *)malloc(sizeof(Queue));
    
    if(q == NULL){
      fprintf(stderr, "ERROR: Fail to malloc\n");
      return NULL;
    }
    
    // initialize variables
    q->enqueueCount = 0;
    q->dequeueCount = 0;
    q->enqueueBlockCount = 0;
    q->dequeueBlockCount = 0;
    q->size = 0;
    q->front = 0;
    q->done = 0;
    
    q->buffer = (char **)malloc(MAXSIZE *sizeof(char*)); // (q->buffer)[MAXSIZE][]
    
    for(i = 0; i<MAXSIZE; i++){
      (q->buffer)[i] = malloc(MAXCHAR *sizeof(char)); // (q->buffer)[MAXSIZE][MAXCHAR]
    }
    
    /*
      sem_init
    */
    if(0 > sem_init(&(q->mutex), 0, 1)){
      fprintf(stderr, "ERROR: Fail to initialize q->mutex sem_t");
    }
    if(0 > sem_init(&(q->empty), 0, MAXSIZE)){ 
      fprintf(stderr, "ERROR: Fail to initialize q->empty sem_t");
    }
    if(0 > sem_init(&(q->full), 0, 0)){ 
      fprintf(stderr, "ERROR: Fail to initialize q->full sem_t");
    }
    if(0 > sem_init(&(q->block), 0, MAXSIZE)){ 
      fprintf(stderr, "ERROR: Fail to initialize q->block sem_t");
    }
  }
  
  
  // return pointer to the queue
  return q;
}
/*
  This function places the pointer to the string at the end of queue q. 
  If the queue is full, then this function blocks until there is space available.
*/ 
void EnqueueString(Queue *q, char *string){// empty line is valid?
  
  // invalid parameter
  if(string == NULL){
    return;
  }
  
  sem_wait(&q->empty); // wait for space
  sem_wait(&q->mutex); // start: critical section
  
  // q->buffer is full 
  if(q->size == MAXSIZE){
  
    (q->enqueueBlockCount)++; 
      
  }else if(q->size < MAXSIZE){
    
    (q->buffer)[(q->front + q->size) % MAXSIZE] = string;
    (q->enqueueCount)++;
    (q->size)++;
    
    sem_post(&q->mutex); // end: critical section
    sem_post(&q->full);  // signal for space
    sem_post(&q->block);
  }
}

/*
  This function removes a pointer to a string from the beginning of queue q. 
  If the queue is empty, then this function blocks until there is a string placed into the queue. 
  This function returns the pointer that was removed from the queue.
*/
char *DequeueString(Queue *q){
  char *result;
  
  sem_wait(&q->full);  // wait until there is data to be dequeued
  sem_wait(&q->mutex); // start: critical section
 
  if(q->size <= 0){ 
  
    (q->dequeueBlockCount)++;
 
    return NULL;
  }else{
     
    // return the string at the most-front index, then increase size
    result = (q->buffer)[q->front];
    q->front = (q->front + 1)%MAXSIZE;
    (q->dequeueCount)++;
    (q->size)--;
    
    sem_post(&q->mutex); // end: critical section
    sem_post(&q->empty); // signal for space
    sem_post(&q->block);
  }
  return result;
}

/*
  This function prints the statistics for this queue
*/
void PrintQueueStats(Queue *q){
  printf("enqueueCount: %d\ndequeueCount: %d\nenqueueBlockCount: %d\ndequeueBlockCount: %d\n", q->enqueueCount, q->dequeueCount, q->enqueueBlockCount, q->dequeueBlockCount);
  
}
