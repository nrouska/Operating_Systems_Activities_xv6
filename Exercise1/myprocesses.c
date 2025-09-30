#include <stdio.h> //for printf()
#include <stdlib.h> //for atoi(),exit()
#include <unistd.h> //for fork()
#include <sys/wait.h> //for wait()

int child_sum(int i, int n, int N){
  long partial_sum=0;
  int range =N/n;
  int start=(i*range)+1;//i=[0,n-1]
  int end= (i==n-1) ? N: (i+1)*range;
  for(int k=start;k<=end;k++){
    partial_sum+=k;
  }
  
return partial_sum;
}  
  
void child_creator(int n,int N,int *fd){
  int pid;
  for(int i=0;i<n;i++){
    pid=fork();//creates copy of calling process
    if (pid==0){//child process
      int sum=child_sum(i,n,N);//calculates and returns partial sum 
      printf("Hello from child %d with sum %d\n",i,sum);
      close(fd[0]);
      write(fd[1],&sum,sizeof(sum));//writes partial sum in fd[1] of pipe
      close(fd[1]);
      exit(0);//terminates child process
    }
    
    else if(pid>0){//parent process
      for(int i=0;i<n;i++){
        wait( NULL);//waits all children to terminate
      }
    } 
  }
}  

  
int main(int argc, char* argv[]){
  int N,n,sum=0,process_sum=0,buffer;
  int fd[2];//file descriptor 
  pipe(fd); //inter-process-communication with pipe, fd[0] read end, fd[1] write end
  if(argc!=3) {
    printf("The arguments must be 3. Try again\n");
  }
  else{
    N=atoi(argv[1]); //converts string to integer
    n=atoi(argv[2]);
    printf("Count the sum of N=%d integers using n=%d processes\n",N,n);
    child_creator(n,N,fd);
  }
  close(fd[1]);// close write end of pipe
  for(int i=0;i<n;i++){
    read(fd[0],&buffer,sizeof(buffer));//read from fd[0] into the buffer
    process_sum+=buffer;
  }
  close(fd[0]); //close read end of pipe
  sum=N*(N+1)/2;
  printf("The sum counted by the parent reading the pipe is %d\n",process_sum);
  printf("The sum counted by the parent is %d\n",sum);
  return 0;
}
