//Won Jun Cho, Yoodong Lee
//wcho8, won || ydlee399, yoodong
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "munch1.h"
#include "queue.h" 

void* to_aster(void *q){

    // cast q into Queue**
    Queue** queues = (Queue**) q;

    char *str = NULL;
    char *curr= NULL;
    
    // change string in the queue by replacing " " to "*"
    while(queues[0]->size != 0 || queues[0]->done != 1){
        if (NULL == (str = DequeueString(queues[0]))){
            fprintf(stderr, "ERROR: Fail to get string from dequeueString.\n");
            return NULL;
        }
        
        // get index for ' '
        curr = index(str, ' ');
        
        // loop until there is no ' '
        while(NULL != curr){
            *curr = '*';
            curr = index(str, ' ');
        }
        
        // enqueue changed string
        EnqueueString(queues[1], str);
    }
    
    // signal done with the Queue
    queues[1]->done = 1;
    
    // exit thread
    pthread_exit((void*)q);
}
