#include "threadfunctions.h"

Buffer buffer;

void add_to_buffer(long double var[]){
    pthread_mutex_lock(&buffer.mutex); // Entered Critical Section
    while(buffer.count== BUFFER_SIZE){
        pthread_cond_wait(&buffer.not_full,&buffer.mutex);
    }
    for (int i=0;i<4;i++){
        buffer.CPU[buffer.write_index][i]=var[i];
    }
    buffer.write_index = (buffer.write_index + 1) % BUFFER_SIZE;
    buffer.count++;
    pthread_cond_signal(&buffer.not_empty);
    pthread_mutex_unlock(&buffer.mutex);
}

void remove_from_buffer(long double var[]){
    pthread_mutex_lock(&buffer.mutex);

    while (buffer.count == 0) {
        pthread_cond_wait(&buffer.not_empty, &buffer.mutex);
    }

    for (int i = 0; i < 4; i++) {
        var[i] = buffer.CPU[buffer.read_index][i];
    }

    buffer.read_index = (buffer.read_index + 1) % BUFFER_SIZE;
    buffer.count--;

    pthread_cond_signal(&buffer.not_full);
    pthread_mutex_unlock(&buffer.mutex);
}