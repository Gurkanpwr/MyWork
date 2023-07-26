#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

//problem 5.3

int counter=0;
pthread_mutex_t mutex;
typedef struct values{
    int counter;

}vls;
void *threads(void *arg){
   vls *ccount=(vls *)arg;
   //int counter=ccount->counter;
    for(int i=0;i<1000000;i++){
       pthread_mutex_lock(&mutex);
       counter++;
      pthread_mutex_unlock(&mutex);
    }
    //ccount->counter=counter;

    pthread_exit(NULL);
    return NULL;
}

int main(){
    int k=10;
    pthread_t td[10];
    vls count;
    int errtest;
    count.counter=0;
    for(int i=0;i<k;i++)

    {
        errtest=pthread_create(&td[i],NULL,threads,(void*)&count);
        if(errtest !=0){
        fprintf(stderr,"Error while creating thread\n");
        }

    }
    for(int i=0;i<10;i++){
        pthread_join(td[i],NULL);
    }
    printf("%d",counter);

}
