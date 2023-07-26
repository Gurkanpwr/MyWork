#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>


//5.4
int counter=0;
sem_t mutex;
void *threads(void *arg){


    for(int i=0;i<1000000;i++){
        sem_wait(&mutex);
        counter++;
        sem_post(&mutex);
    }

    pthread_exit(NULL);
}
int main(){
    int k=10;
    pthread_t td[10];
    int errtest;
    sem_init(&mutex,0,1);
    for(int i=0;i<k;i++)

    {
        errtest=pthread_create(&td[i],NULL,threads,NULL);
        if(errtest !=0){
            fprintf(stderr,"Error while creating thread\n");
        }

    }
    for (int i=0;i<10;i++){
        pthread_join(td[i],NULL);
    }
    printf("%d",counter);
    sem_destroy(&mutex);
    return 0;
}