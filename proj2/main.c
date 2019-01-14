//Won Jun Cho, Yoodong Lee
//wcho8, won || ydlee399, yoodong
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "reader.h"
#include "queue.h"
#include "munch1.h"
#include "munch2.h"
#include "writer.h"

#define T_SIZE 4
#define Q_SIZE 4
#define BUFFER 1024 // need to change later

int main(){
	pthread_t threads[T_SIZE]; // 0: reader 1: munch1 2: munch2 3: writer
	void *q[Q_SIZE];
	int i = 0; // for initializing queue
	sem_t ready;


  // initialize q[]
	while(i < Q_SIZE-1){
    q[i] = CreateStringQueue(BUFFER);
    
		if(NULL == q[i]){
			fprintf(stderr, "ERROR: Fail to initialize queue. \n");
			return -1;
		}
		i++;
	}
	
	// initialize semaphore 
	if(0 > sem_init(&ready, 0, 1)){
		fprintf(stderr, "ERROR: Fail to initialize semaphore. \n");
		return -1;
	}
 
  q[3] = &ready;
 
	// initialize threads
	if(pthread_create(&threads[0], NULL, &read, q)){
		fprintf(stderr, "ERROR: Fail to create reader thread. \n");
		return -1;
	}

	if(pthread_create(&threads[1], NULL, to_aster, q)){
		fprintf(stderr, "ERROR: Fail to create munch1 thread. \n");
		return -1;
	}

	if(pthread_create(&threads[2], NULL, toUpper, q)){
		fprintf(stderr, "ERROR: Fail to create munch2 thread. \n");
		return -1;
	}

	if(pthread_create(&threads[3], NULL, writeOut, q)){
		fprintf(stderr, "ERROR: Fail to create writer thread. \n");
		return -1;
	}

	//join the threads
	if(pthread_join(threads[0], NULL)){
		fprintf(stderr, "ERROR: Fail to join the reader thread. \n");
		return -1;
	}

	if(pthread_join(threads[1], NULL)){
		fprintf(stderr, "ERROR: Fail to join the munch1 thread. \n");
		return -1;
	}

	if(pthread_join(threads[2], NULL)){
		fprintf(stderr, "ERROR: Fail to join the munch2 thread. \n");
		return -1;
	}

	if(pthread_join(threads[3], NULL)){
		fprintf(stderr, "ERROR: Fail to join the writer thread. \n");
		return -1;
	}

	//free queues and semaphores
 
  int j;
  for(j=0; j<Q_SIZE-1; j++){
    sem_destroy(&((Queue*)q[j])->empty);
    sem_destroy(&((Queue*)q[j])->full);
    sem_destroy(&((Queue*)q[j])->mutex);
  }
  sem_destroy(q[3]);
  free((void*)q[0]);
  free((void*)q[1]);
  free((void*)q[2]);
 // free((void*)q[3]); 
  
  
  pthread_exit(NULL);
	return 0;
}
