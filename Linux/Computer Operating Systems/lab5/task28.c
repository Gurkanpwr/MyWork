#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
void Child_Process(int* pip){
    dup2(pip[1],STDOUT_FILENO);
    execl("/usr/bin/cal", "cal", NULL);
    close(pip[1]);
}
int main() {
    char buffer[10000]={};
    int pip[2],status;
    pid_t pid;
    if(pipe(pip)<0){
        fprintf(stderr,"Error Creating pipe!");
        exit(EXIT_FAILURE);
    }
    if((pid=fork())<0){
        fprintf(stderr,"Error Creating Child!");
        exit(EXIT_FAILURE);
    }
    else if(pid==0){
        Child_Process(pip);
        exit(EXIT_SUCCESS);
    }