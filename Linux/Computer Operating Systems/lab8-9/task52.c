#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

//problem 5.2

int glob_counter=0;

void *threads(void *arg){

    for(int i=0;i<1000000;i++){

    glob_counter++;

}

   pthread_exit(NULL);
}
int main(){
    pthread_t td[10];
    int err;

    for(int i=0;i<10;i++){
       if((err= pthread_create(&td[i],NULL,threads,NULL)) !=0){
           fprintf(stderr,"Error while creating thread\n");
       }
    }
    for(int i=0;i<10;i++){
    pthread_join(td[i],NULL);}
    printf("%d",glob_counter);
}
