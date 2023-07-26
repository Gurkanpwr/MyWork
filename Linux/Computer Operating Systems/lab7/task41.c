#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

void Child_Proccess(int Sh_mem,int size){

    char* Child_Map= mmap(NULL,size,PROT_READ,MAP_SHARED,Sh_mem,0);
    if(Child_Map<0){
        fprintf(stderr,"Error in Child Process While Shm_Open");
        exit(EXIT_FAILURE);
    }
    printf("CHILD: Message that I received: %s",Child_Map);
}
int main(){
    int status;
    pid_t pid;
    char Message[]="Hello!";
    const char* Shared_Mem_Name="/New Memory";
    int Shared_fd=shm_open(Shared_Mem_Name,O_CREAT|O_RDWR,0777);
    if(Shared_fd<0){
        fprintf(stderr,"Error occured Creating Shared Memory!");
        return 1;
    }
    if(ftruncate(Shared_fd, sizeof(Message))<0){
        fprintf(stderr,"Error Allocating shared memory space!");
        return 2;
    }
    if((pid=fork())<0){
        fprintf(stderr,"Error Creating Child Process");
        return 3;
    }
    else if(pid==0){
        Child_Proccess(Shared_fd,sizeof(Message) );
        exit(EXIT_SUCCESS);
    }
    char *Shared_Mmap= mmap(NULL,sizeof (Message),PROT_WRITE,MAP_SHARED,Shared_fd,0);
    if(Shared_Mmap==MAP_FAILED){
        fprintf(stderr,"Error Creating Memory Map");
        return 4;
    }
    strcpy(Shared_Mmap,Message);
    shm_unlink(Shared_Mem_Name);
    wait(&status);
    munmap(Shared_Mmap,sizeof (Message));
    return 0;

}