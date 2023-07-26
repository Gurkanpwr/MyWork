#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

void *thread(void *arg) {
    //char *ret;
    for(int i=1;i<=10;i++){

        printf("%d\n",i);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t thread1;


    if (pthread_create(&thread1, NULL, thread, NULL) != 0) {
        perror("pthread_create() error");
        exit(1);
    }

    if (pthread_join(thread1, NULL) != 0) {
        perror("pthread_create() error");
        exit(3);
    }
    return 0;
}