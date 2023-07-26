#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"
#include <sys/types.h>
#include <sys/wait.h>
#include "string.h"
#define pipfd 2
//Lab 4
void Child_Proc(int* pipe_line_child){
    char* child_data="";
    read(pipe_line_child[0],&child_data, sizeof(child_data));
    printf("CHILD is speaking!\n");
    printf("%s",child_data);
    close(pipe_line_child[0]);
}
int main(){
    int pipe_line[pipfd];
    if(pipe(pipe_line)<0){
        fprintf(stderr,"Error creating Pipe\n");
        exit(EXIT_FAILURE);
    }
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"Fork Error\n");
        exit(EXIT_FAILURE);
    }
    else if(pid==0){
        Child_Proc(pipe_line);
    }
    //parent Proccess
    char* Parrent_message="Hello this is message from Parent";
    write(pipe_line[1],&Parrent_message,sizeof (Parrent_message));
    close(pipe_line[1]);
    int status;
    wait(&status);
    close(pipe_line[1]);
    exit(0);
}
