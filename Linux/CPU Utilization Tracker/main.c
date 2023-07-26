#include "threadfunctions.h"

#define thread_num 5


extern pthread_t threads[thread_num];
extern Buffer buffer;
int main() {
    unit_test();
    /*
     * Initialise buffer
     **/
    buffer.read_index=0;
    buffer.write_index=0;
    buffer.count=0;
/*
 * Initialise mutex and condition varriables
 * */
    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.not_empty, NULL);
    pthread_cond_init(&buffer.not_full, NULL);

    int rc;
    remove("CPUlog.txt");


    rc = pthread_create(&threads[0], NULL, Reader, NULL);
    if (rc) {
        perror("Pthread error for Reader thread");
        exit(EXIT_FAILURE);
    }

    rc = pthread_create(&threads[1], NULL, Analyzer, NULL);
    if (rc) {
        perror("Pthread error for Analyzer thread");
        exit(EXIT_FAILURE);
    }

    rc = pthread_create(&threads[2], NULL, Printer, NULL);
    if (rc) {
        perror("Pthread error for Printer thread");
        exit(EXIT_FAILURE);
    }

    rc= pthread_create(&threads[3],NULL,Watchdog,NULL);
    if(rc){
        perror("Pthread error for Watchdog thread");
    }
    rc= pthread_create(&threads[4],NULL, Logger,NULL);
    if(rc){
        perror("Error occured at Logger thread");
    }
    //Sigterm
    struct sigaction sa;
    sa.sa_handler = sigterm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);

    for (int i = 0; i < thread_num ; i++) {
        rc = pthread_join(threads[i], NULL);
        if (rc) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&buffer.mutex);
    pthread_cond_destroy(&buffer.not_empty);
    pthread_cond_destroy(&buffer.not_full);
    return 0;
}
