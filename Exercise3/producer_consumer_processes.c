#include <stdio.h>//printf()
#include <stdlib.h>//exit()
#include <unistd.h>//fork()
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h> // O_constants
#define N 4
//each process has each own address space ; explicit shared memory is required for IPC 

int *buffer_shm;
sem_t *empty;
sem_t *full;
sem_t *mutex;
int *in_shm=0;
int *out_shm=0;

void producer(){
  while(1){
    int item= rand() %10; //produce item
    sem_wait(empty); //wait until empty slot in buffer exists empty>0
    sem_wait(mutex); //lock buffer
    buffer_shm[*in_shm]=item; //add item in buffer
    printf("producer %d produced %d at index %d\n",getpid(),item,*in_shm);
    *in_shm=(*in_shm+1)%N; //increment index circurarly
    sem_post(mutex); //unlock buffer
    sem_post(full); //notify consumer for full slot
    sleep(2);
  }
}

void consumer(){
  while(1){
   
    sem_wait(full); // wait until full slot in buffer exists full>)
    sem_wait(mutex); //locks buffer
    int item= buffer_shm[*out_shm]; //consume item
    buffer_shm[*out_shm]=0; // remove item from buffer
    printf("consumer %d consumed %d at index %d\n",getpid(),item,*out_shm);
    *out_shm=(*out_shm+1)%N; //increment index circularly
    sem_post(mutex); //unlock buffer
    sem_post(empty); //notify producer for empty slot
    sleep(2);
  }
}
  
int main(){
  //create memory segments (buffer, in, out) and attach them to pointers  
  int buffer_shm_id =shmget(0123,N*sizeof(int),0666|IPC_CREAT); // return id of the memory segment
  //0666: read and write permissions for all users
  buffer_shm = shmat(buffer_shm_id, NULL, 0);
  
  int in_shm_id =shmget(4567,sizeof(int),0666|IPC_CREAT); 
  
  in_shm = shmat(in_shm_id, NULL, 0);
  
  int out_shm_id =shmget(8910,sizeof(int),0666|IPC_CREAT);
  
  out_shm = shmat(out_shm_id, NULL, 0);
  // now buffer, in , out are pointers to shared memory
  *in_shm=0; //value of the variable the pointer points to
  //create semaphores and initialize them
  mutex=sem_open("mutex_sem",O_CREAT,0666,1);
  empty=sem_open("empty_sem",O_CREAT,0666,N);
  full=sem_open("full_sem",O_CREAT,0666,0);
  
  
  for(int i=0;i<2;i++){
  
    if(fork()==0){ //child process
      producer();
      exit(0);
    }
    
}
    
    
  for(int i=0;i<2;i++){
  
    if(fork()==0){ //child process
      consumer();
      exit(0);
    }
    
 }
 
  wait(NULL); //parent waits for all processes to terminate

  
}

