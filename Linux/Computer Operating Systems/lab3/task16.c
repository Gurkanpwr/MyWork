#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"
#include <sys/types.h>
#include <sys/wait.h>
void parentProc(pid_t pid){
    int status;
    waitpid(pid,&status,0);
    if(WIFEXITED(status)){
        printf("%d", WEXITSTATUS(status));
    }
}
void childProc(){
    int a,b;
    srand(time(NULL));
    a = rand() % 11;
    b = rand() % 11 + 20;
    printf("%d+%d=",a,b);
    exit(a+b);
}
int main(){
    pid_t pid;
    pid=fork();
    if(pid<0){
        fprintf(stderr,"Error\n");
        return 1;
    }
    else if(pid==0){
        childProc();
    }
    else{
        parentProc(pid);
    }

}