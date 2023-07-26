#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "wait.h"

//task 1.3
void child_Proc(){
    printf("CHILD: My pid:%d \n",getpid());
    // printf("CHILD: MY parrent ID: %d\n",getppid());
    exit(0);
}
void parent_Proc(){

    printf("PARENT: My pid:%d\n",getpid());
    wait(NULL);
}
int main(){
    pid_t pid;
    for(int n=1;n<=5;n++){
        pid=fork();
        if(pid<0){
            fprintf(stderr,"Fork Error\n");
            return 1;
        }
        else if(pid==0){
            child_Proc();
        }
        else{
            wait(NULL);
        }

    }
}