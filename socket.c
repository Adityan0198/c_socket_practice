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
#define BACKLOG 5

int main(){
    struct addrinfo hint;

    memset(&hint, 0, sizeof hint);
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;

    struct addrinfo *servinfo;

    int status;
    if ((status = getaddrinfo(NULL, PORT, &hint, &servinfo)) != 0){
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 1;
    }

    int sockfd;
    //Find what we can bind first to in servinfo
    for (struct addrinfo* p = servinfo; p != NULL; p = p->ai_next){
        //If couldn't get socket
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("socket Error");
        } else if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("Bind Error");
        } else {
            break;
        }
    }

    freeaddrinfo(servinfo); //done with it

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen error");
        return 1;
    }

    struct sockaddr_storage usr_addr;
    socklen_t usr_addr_size = sizeof usr_addr;

    int accfd = accept(sockfd, (struct sockaddr*) &usr_addr, &usr_addr_size);
    if (accfd == -1){
        perror("Accept req error");
        return 1;
    }
    printf("Got a connection\n");

    //Send data
    char* msg = "Got Data!\n";
    int sent = send(accfd, msg, strlen(msg), 0);
    printf("Sent %d bytes", sent);

    close(accfd);
    close(sockfd);
    return 0;
}