#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"
#include <sys/types.h>
#include <sys/wait.h>
#include "string.h"
#include "ctype.h"
#define pipfd 2
//Lab 4
void Child_Proc(int* parent_to_child,int* child_to_parent){
    char child_data[8000];//buffer
    while(1){
        int byte= read(parent_to_child[0],child_data,sizeof (child_data));
        for(int i=0;i<byte;i++){
            if(child_data[i]>='a'&&child_data[i]<='z') {
                child_data[i] = toupper(child_data[i]);
            }
        }
        printf("CHILD: %s",child_data);
        write(child_to_parent[1],child_data,byte);
    }

}
int main(){
    char buffer[8000];
    int parent_pipe[pipfd],child_pipe[pipfd];
    if(pipe(parent_pipe)<0|| pipe(child_pipe)){
        fprintf(stderr,"Error creating Pipe\n");
        exit(EXIT_FAILURE);
    }
    pid_t pid=fork();
    if(pid<0){
        fprintf(stderr,"Fork Error\n");
        exit(EXIT_FAILURE);
    }
    else if(pid==0){
        Child_Proc(parent_pipe,child_pipe);
    }
    //parent Proccess

    char Parrent_message[2][100]={"Hello this is message from Parent",
                                  "This time We are looking how our child works"};
    printf("Received Message!: ");
    for(int i=0;i<2;i++){
        write(parent_pipe[1],Parrent_message[i], sizeof(Parrent_message[i]));
        close(parent_pipe[1]);
        read(child_pipe[0],buffer, sizeof(buffer));
        close(child_pipe[0]);
        printf("%s\n",buffer);
    }
    int status;
    wait(&status);
    exit(0);
}
