#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

void Client(char *msg){
    double res;
    struct sockaddr_in  serv_addr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0){
        fprintf(stderr,"Socket Can't created from the Client");
        exit(EXIT_FAILURE);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    int sd = sendto(sockfd, msg,
                    strlen(msg),
                    0, (const struct sockaddr *)&serv_addr,
                            sizeof(serv_addr));

    if (sd < 0) {
        fprintf(stderr,"Error in Send from Client");
        exit(EXIT_FAILURE);
    }
    //Time to receive from Server
    sd= recvfrom(sockfd,
                 &res,
                 sizeof(res),
                 0,NULL,NULL);
    if(sd<0){
        fprintf(stderr,"Receiving error");
        exit(EXIT_FAILURE);
    }
    printf("%f",res);
    }
    int main(int argc,char *argv[]){
    if(argc !=2){
        fprintf(stderr,"Error because of input!");
        exit(EXIT_FAILURE);
    }
    Client(argv[1]);
    exit(EXIT_SUCCESS);

}

