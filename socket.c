#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3005" 

int main(){
    struct addrinfo hint;

    memset(&hint, 0, sizeof hint);
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;

    struct addrinfo *servinfo;

    int status;
    if (status = getaddrinfo(NULL, PORT, &hint, &servinfo) != 0){
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 1;
    }

    int sockfd;
    //Find what we can bind to in servinfo
    for (struct addrinfo* p = servinfo; p != NULL; p = p->ai_next){
        //If couldn't get socket
        if (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol) == -1){
            perror("socket Error");
        } else if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("Bind Error");
        } else {
            break;
        }
    }

    freeaddrinfo(servinfo); //done with it

    return 0;
}