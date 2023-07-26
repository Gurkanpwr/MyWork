#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define Size_of_Array        (unsigned)1000000
#define Proc_Number        (unsigned)10
#define Per_Array_Size    (unsigned)(Size_of_Array / Proc_Number)

float child_Process(const float const* array, unsigned arr_begin) {
    float sum = 0;
    char file_name[20];
    for (unsigned i = arr_begin; i < arr_begin + Per_Array_Size; ++i) {
        sum += array[i];
    }
    sprintf(file_name, "sum%i.txt", getpid());
    int file = creat(file_name, 0666);
    if (file < 0) {
        fprintf(stderr,"Error occured while creating a file");
        exit(EXIT_FAILURE);
    }

    /* Write sum to file */
    write(file, &sum, sizeof(sum));

    close(file);
    exit(getpid());
}

int main() {
    float array_results;
    char file_name[20];//file names whic are created by child
    float* array = (float*)malloc(sizeof(float)*Size_of_Array);
    srand(time(NULL));
    for (unsigned i = 0; i < Size_of_Array; ++i) {
        array[i] = 2*((float)rand()/RAND_MAX)-1;//crating random float number between -1,1;
    }

    pid_t pids[Proc_Number];
    for(unsigned i = 0; i < Proc_Number; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            exit(1);
        }

        if (pid == 0) {
            child_Process(array, i * Per_Array_Size);
            exit(0);
        }
        pids[i] = pid;
    }

    int status;
    float sum = 0;
    for (unsigned i = 0; i < Proc_Number; ++i) {

        waitpid(pids[i], &status, 0);


        if (WIFEXITED(status)) {

            sprintf(file_name, "sum%i.txt", pids[i]);
            int file = open(file_name, O_RDONLY);
            if (file < 0) {
                perror("File");
                exit(EXIT_FAILURE);
            }


            read(file, &array_results, sizeof(array_results));
            sum += array_results;

            close(file);
        }
    }

    float avg = sum / Size_of_Array;
    printf("Mean 1000000 of random float numbers between -1 and 1 : %.5f\n", avg);

    exit(0);
}
