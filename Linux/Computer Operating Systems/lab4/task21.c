#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"
#include <sys/types.h>
#include <sys/wait.h>
#define pipfd 2
//Lab 4

int main(){
    int fd[pipfd];
    if(pipe(fd)<0){
        fprintf(stderr,"ERROR creating Pipe");
    }
    int input=20;
    int output=0;

    write(fd[1],&input,sizeof (input));
    close(fd[1]);
    read(fd[0],&output,sizeof (input));
    close(fd[0]);
    printf("Input: %d\nOutput= %d\n",input,output);

}