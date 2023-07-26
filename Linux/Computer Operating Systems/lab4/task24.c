#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

#define ARR_SIZE        (unsigned)1000000
#define PIDS_NUM        (unsigned)10
#define PID_ARR_SIZE    (unsigned)(ARR_SIZE / PIDS_NUM)

void child_task(const float const* arr, unsigned arr_begin, int pipe) {
    float sum = 0.0;
    for (unsigned i = arr_begin; i < arr_begin + PID_ARR_SIZE; ++i) {
        sum += arr[i];
    }

    write(pipe, &sum, sizeof(sum));
    close(pipe);
}

float rand_float(float min, float max) {
    return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}

float average(const float const* arr) {
    float sum = 0.0;
    for (unsigned i = 0; i < PIDS_NUM; ++i) {
        sum += arr[i];
    }
    return sum / ARR_SIZE;
}

int main() {
    float* arr = (float*)malloc(sizeof(float)*ARR_SIZE);

    /* Generate array of random floats from -1 to 1 */
    srand(time(NULL));
    float min = -1.0, max = 1.0;
    for (unsigned i = 0; i < ARR_SIZE; ++i) {
        arr[i] = rand_float(min, max);
    }

    int child_to_parent[PIDS_NUM][2];
    for (unsigned i = 0; i < PIDS_NUM; ++i) {
        if (pipe(child_to_parent[i]) == -1) {
            perror("Pipe");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pids[PIDS_NUM];
    for(unsigned i = 0; i < PIDS_NUM; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            child_task(arr, i * PID_ARR_SIZE, child_to_parent[i][1]);
            exit(0);
        }
        pids[i] = pid;
    }

    /* Read data from pipes */
    float sums[PIDS_NUM];
    for (unsigned i = 0; i < PIDS_NUM; ++i) {
        read(child_to_parent[i][0], &sums[i], sizeof(sums[i]));
        close(child_to_parent[i][0]);
    }
    printf("Average of sums: %.5f\n", average(sums));

    exit(0);
}
