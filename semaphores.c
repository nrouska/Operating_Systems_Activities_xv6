#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
int a2,b1,c1,c2,x,y,z;
sem_t sem[4];
void* t1(){
  int a1=10;
  a2=11;
  sem_post(&sem[2]);//signal a2 is initialized
  
  sem_wait(&sem[1]);//wait until z is computed and c2,c1 are initialized
  y=a1+c1;
  printf("y is %d\n",y);
  sem_post(&sem[2]);//signal y is computed
  sem_wait(&sem[0]);//wait until x is computed
  printf("x is %d\n",x);
}
void* t2(){
  b1=20;
  int b2=21;
  sem_post(&sem[2]);//signal b1 is initialized
  
  sem_wait(&sem[1]);//wait until z is computed and c2,c1 are initialized
  sem_wait(&sem[2]);//wait until y is computed
  int w=b2+c2;
  printf("w is %d\n",w);
  x=z-y+w;
  sem_post(&sem[0]);//signal x is computed
}
void* t3(){
  c1=30;
  c2=31;
  sem_wait(&sem[2]);//wait until b1 is initialized
  sem_wait(&sem[2]);//wait until a2 is initialized
  
  z=a2+b1;
  printf("z is %d\n",z);
  sem_post(&sem[1]);//signal z is computed and c2,c1 are initialized
  sem_post(&sem[1]);//signal z is computed and c2,c1 are initialized
}
int main(int argc,char* argv[]){
  pthread_t th[3];
  for(int i;i<3;i++){
    sem_init(&sem[i],0,0);
  }

  if((pthread_create(&th[0],NULL,&t1,NULL)<0)){
    printf("Error creating thread 0");
  }
  if((pthread_create(&th[1],NULL,&t2,NULL)<0)){
    printf("Error creating thread 1");
  }
  if((pthread_create(&th[2],NULL,&t3,NULL)<0)){
    printf("Error creating thread 3");
  } 
  pthread_join(th[0],NULL);  
  pthread_join(th[1],NULL);
  pthread_join(th[2],NULL);

}
  
