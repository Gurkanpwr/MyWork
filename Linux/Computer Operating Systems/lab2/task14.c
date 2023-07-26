#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "wait.h"


//task 14
void child_Proc(){
    sleep(2);
    printf("CHILD: My pid:%d \n",getpid());
    printf("CHILD: MY parrent ID: %d\n",getppid());

}
void parent_Proc(){
    //sleep(2);
    printf("PARENT: My pid:%d\n",getpid());
    wait(NULL);
}
int main(){
    pid_t pid;
    if((pid=fork())<0){
        fprintf(stderr,"fork error");
        return 1;
    }
    else if(pid>0){
        parent_Proc();
    }
    else{
        child_Proc();
    }
}
