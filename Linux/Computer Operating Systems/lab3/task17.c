#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"
#include <sys/types.h>
#include <sys/wait.h>
# define Size 1000000
void childProc(int* A){
    int tot=0;
    int nsize=100000;
    for (int i=0;i<nsize;i++){
        tot+=A[i];
    }
    tot=tot/nsize;
    exit(tot);
}
int main(){
    pid_t pid;
    int status,avg;
    int *A=(int*) malloc(sizeof (int)*Size);
    srand(time(NULL));
    for(int i=0;i<Size;i++){
        A[i]=rand()%100;//max value 100 min 0
    }
    pid_t pids[10];
    for(int i=0;i<10;i++){
        if((pid=fork())<0){
            fprintf(stderr,"Error\n");
            return 1;
        }
        else if(pid==0){
            childProc(A);
            exit(0);
        }
        pids[i]=pid;
    }

    //PARENT AREA
    for(int i=0;i<10;i++){
        waitpid(pids[i],&status,0);
        if(WIFEXITED(status)){
            int k=WEXITSTATUS(status);
            avg+= k;
            printf("%d.Child:My Result=%d\n",i+1,k);
        }
    }

    printf("Total Average :%d ",avg/10);


}