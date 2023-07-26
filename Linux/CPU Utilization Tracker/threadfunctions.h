//
// Created by gurkan on 28.03.23.
//
#include "pthread.h"
#include "stdio.h"
#include "unistd.h"
#include "signal.h"
#include <sys/time.h>
#include "stdlib.h"
#include "string.h"
#include <errno.h>
#include "assert.h"
#include <stdbool.h>
#include "time.h"

#ifndef CUT_THREADFUNCTIONS_H
#define CUT_THREADFUNCTIONS_H

#define BUFFER_SIZE 10
void* Reader(__attribute__((unused)) void* p );
void* Analyzer(__attribute__((unused)) void* p);
void* Printer(__attribute__((unused)) void* p);
void* Watchdog(__attribute__((unused)) void* p);
void* Logger(__attribute__((unused)) void*);
void sigterm_handler(__attribute__((unused)) int sig);

void progress_bar(float data);
int array_comp(const long double* a,const long double* b,int size); // it will check if the analyzer gets new data from Reader.
void unit_test(void);

void add_to_buffer(long double var[]); //it will add variables to buffer
void remove_from_buffer(long double var[]);// it removes variables from buffer

typedef struct {
    long double CPU[BUFFER_SIZE][4];
    int read_index;
    int write_index;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
}Buffer;

#endif //CUT_THREADFUNCTIONS_H
