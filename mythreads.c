#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
struct thread_data{
  long id;
  long sum;
  long start;
  long end;
  
};
void* thread_count_sum(void* thread){

  struct thread_data* mythread = (struct thread_data*) thread; 
  mythread->sum=0;
  
  for(int k=mythread->start;k<=mythread->end;k++){
    mythread->sum+=k;
  }
  printf("Hello from thread %ld with sum %ld\n",mythread->id,mythread->sum); 
  pthread_exit((void*)mythread->sum);
  return NULL;
}

void thread_creator(struct thread_data* thread_data_array,int n,int N){
  pthread_t threads[n];
  int range =N/n;
  void * returnval;
  int total_sum=0;
  for(long i=0;i<n;i++){//
    thread_data_array[i].id=i;
    thread_data_array[i].start=(i*range)+1;
    thread_data_array[i].end = (i==n) ? N: (i+1)*range;
    //creates and starts a thread form calling process
    pthread_create(&threads[i],NULL,thread_count_sum,(void*)&thread_data_array[i]);
    
  }
  for(long i=0;i<n;i++){
  //blocks the calling thread until thread with id threads[i] terminate
    pthread_join(threads[i],&returnval);
    
  }  
}  
  
int main(int argc, char* argv[]){
  int N,n,sum=0,thread_sum=0;
  if(argc!=3) {
    printf("The arguments must be 3. Try again\n");
  }
  else{
    N=atoi(argv[1]);//convert string argument to integer
    n=atoi(argv[2]);
    printf("Count the sum of N=%d integers using n=%d threads\n",N,n);
    struct thread_data mythread_data_array[n];//array with n elements of type thread data
    thread_creator(mythread_data_array,n,N);
    for(long i=0;i<n;i++){
      thread_sum+=mythread_data_array[i].sum;//add the field sum of each element->total sum
    }
  }
  
  sum=N*(N+1)/2;
  printf("The sum counted by the threads is %d\n",thread_sum);
  printf("The sum counted by the parent is %d\n",sum);
  return 0;
}
  
