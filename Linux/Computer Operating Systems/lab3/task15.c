#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "wait.h"

void parentProc(){
    wait(NULL);
    printf("End of Work!\n");
}
void childProc(){
    for(int i=1;i<=10;i++){
        printf("%d ",i);
        fflush(stdout);
        sleep(1);
    }
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
        parentProc();
    }

}