#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "wait.h"

void child_Proc(){
    printf("CHILD: My pid:%d \n",getpid());
    printf("CHILD: MY parrent ID: %d\n",getppid());
}
void parent_Proc(){
    printf("PARENT: My pid:%d\n",getpid());
}
int main(){
//task 12
    pid_t pid;
    pid=fork();
    if(pid<0){
        fprintf(stderr,"ERROR WHILE CREATING FORK!");
        return 1;
    }
    else if(pid==0){
        child_Proc();
    }
    else{
        wait(NULL);
        parent_Proc();
    }
}