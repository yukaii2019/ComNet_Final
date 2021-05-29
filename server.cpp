#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
using namespace std;
/*
struct in_addr{
    uint32_t s_addr;
};
struct sockaddr_in{
    //uint8_t sin_len;
    sa_family_t sin_family;
    uint16_t sin_port;
    struct in_addr sin_addr;
    //char sin_zero[8];
};
*/
void handle_client(int connfd);

int main(){
    struct sockaddr_in myaddr;
    struct sockaddr_in client_addr;
    int sockfd, streamfd, port, status;
    int addr_size;
    char str_buf[100];
    bzero(&myaddr,sizeof(myaddr));
    myaddr.sin_family = PF_INET;
    myaddr.sin_port=htons(1234);
    myaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    sockfd = socket(PF_INET,SOCK_STREAM,0);
    bind(sockfd,(struct sockaddr*) &myaddr,sizeof(struct sockaddr_in));
    listen(sockfd,10);
    addr_size = sizeof(client_addr);
    while(1){
        streamfd = accept(sockfd,(struct sockaddr*) &client_addr,(socklen_t*)&addr_size);
        handle_client(streamfd);
        close(streamfd);
    }
    return 0;
}

void handle_client(int streamfd){
    char buffer[4];
    read(streamfd,buffer,sizeof(buffer));
    cout << buffer << endl;
    cout << "aaaaaaaaaa" << endl;
}
