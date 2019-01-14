//Won Jun Cho, Yoodong Lee
//wcho8, won || ydlee399, yoodong
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
typedef struct Queue{
  int enqueueCount; 
  int dequeueCount;
  int enqueueBlockCount;
  int dequeueBlockCount;
  
  int front; 
  int size;
  int done;
  
  char **buffer;
  
  sem_t empty;
  sem_t full;
  sem_t mutex;
  sem_t block;
  
} Queue;

Queue* CreateStringQueue(int);

void EnqueueString(Queue* , char*);

char* DequeueString(Queue*);

void PrintQueueStats(Queue*);
