//Won Jun Cho, Yoodong Lee
//wcho8, won || ydlee399, yoodong
#include <stdio.h>
#include <stdlib.h>
#include "reader.h"
#include "queue.h"

#define BUFFERSIZE 1024

void* read(void *q){
   Queue** queues = (Queue**)q;
   FILE *file;
   char *input, char_in;
   int char_count, err_1, file_end, line_end;

   file = stdin;
   while(!feof(file)){
     char_count = 0; // keeping track of char count to not exceed the BUFFERSIZE
     err_1 = 0; // if the line is null
     file_end = 0; // end of the file
     line_end = 0; // end of the line

     //if fail to allocate memory, force exit the program
     if(NULL == (input = (char*)malloc(BUFFERSIZE*sizeof(char)))){
       fprintf(stderr, "ERROR: Fail to allocate buffer\n");
       exit(1);
     }

     sem_wait((sem_t*)queues[3]); // semaphore with writer. It says writer to wait

     while((!line_end && !err_1) && !file_end){
       char_in = fgetc(file);
       if(EOF == char_in){
         line_end = 1;
         file_end = 1;
       }else if('\0' == char_in){
         err_1 = 1;
       }else if('\n' == char_in){
         line_end = 1;
       }else if(char_count < BUFFERSIZE){
         input[char_count] = char_in;
         char_count++;
       }
     }

     if(!err_1){
       // if not null and char_count is not bigger than BUFFERSIZE, enqueue the input to queues[0]
       if(char_count < BUFFERSIZE){
         input[char_count]='\0';
         EnqueueString(queues[0], input);
       }else{
         fprintf(stderr, "ERROR: Exceed the Buffer (1024 characters)\n");
         sem_post((sem_t*)queues[3]); // Make sure to not get stuck itself
       }
     }
   }
   
   // signal done with the Queue
   queues[0]->done = 1; 
   fclose(file);
   
   //exit the thread
   pthread_exit((void*)q); 
}
