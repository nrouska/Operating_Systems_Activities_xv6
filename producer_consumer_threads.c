#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N 4
sem_t mutex;//mutual exclusion
sem_t empty;//initially all slots are empty, empty =N
sem_t full;//initially all slots are empty, full=0
int buffer[N];
int in=0; // producer index
int out=0; //consumer index

void  *producer(void* param){
  int i=* ((int *) param); 
  while(1){
  
    int item=rand() % 10;  
    sem_wait(&empty); // decrement empty ; if empty<0 (full buffer) producer in queue (waits for consumer signaL) else enters critical path
    sem_wait(&mutex); // locks mutex, only producer can access buffer now
    buffer[in]=item;
    printf("%d producer produced %d at index %d\n",i, item,in);
    in =(in+1)%N; // increment producer index circurally
    sem_post(&mutex); //unlocks mutex
    sem_post(&full); // increment full ; notify consumer for full slot
    sleep(2);
  }
}
void *consumer(void* param){
  int i=* ((int *) param);
  while(1){
  
    sem_wait(&full);// decrement full ; if full<0 (empty buffer) consumer in queue (waits for producer signal) else enters critical path
    sem_wait(&mutex); // locks mutex, only consumer can access buffer now
    int item=buffer[out];
    buffer[out]=0;
    printf("%d consumed %d at index %d\n", i,item,out);
    out =(out+1)%N; // increment consumer index circurally
    sem_post(&mutex); // unlocks mutex
    sem_post(&empty); //increment empty ; notify producer for empty slot
    sleep(2);
  }
  
}

int main(int argc, char* argv[]){

  sem_init(&mutex,0,1); // initialize mutex semaphore to 1 (1 buffer)
  sem_init(&empty,0,N); // initialize empty semaphore to N (all slots empty)
  sem_init(&full,0,0); // initialize full semaphore to 0 (all slots empty)
  pthread_t producer_th[2];
  pthread_t consumer_th[2];
  int prod_index[2]={1,2};
  int cons_index[2]={1,2};
  
  //create producer threads
  for(int i=0;i<2;i++){
    if(pthread_create(&producer_th[i],NULL,producer,&prod_index[i])<0){
      printf("error creating thread");
    }
  }
  //create consumer threads
   for(int i=0;i<2;i++){
    if(pthread_create(&consumer_th[i],NULL,consumer,&cons_index[i])<0){
      printf("error creating thread");
    }
  }
  // wait for all threads to exit
    for(int i=0;i<2;i++){
      pthread_join(producer_th[i],NULL);
      pthread_join(consumer_th[i],NULL);
      
    }
  
  
  sem_destroy(&full);
  sem_destroy(&mutex);
  sem_destroy(&empty);
  
  return 0;
  
}
  
    
  
