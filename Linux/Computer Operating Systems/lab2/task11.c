#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "wait.h"

void child_Proc(){
    printf("Hello I am CHILD\n");
}
void parent_Proc(){
    printf("Hello I am PARENT\n");
}
int main(){
//task 11
    pid_t pid;
    if((pid=fork())<0){
        fprintf(stderr,"ERROR WHILE CREATING FORK!");
        return 1;
    }
    else if(pid==0){
        child_Proc();
    }
    else{
        parent_Proc();
    }
}