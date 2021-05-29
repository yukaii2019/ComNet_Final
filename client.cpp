#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>

using namespace std;

void Server_Request_Procedure(int sockfd);

int main(){
    struct sockaddr_in server_addr;
    int sockfd,status;
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    
    sockfd = socket(PF_INET,SOCK_STREAM,0);
    connect(sockfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in));
    
    Server_Request_Procedure(sockfd);
    close(sockfd);
    return 0;
}

void Server_Request_Procedure(int sockfd){
    char buffer[4];
    buffer[0] = 'a';
    buffer[1] = 'b';
    buffer[2] = 'c';
    buffer[3] = '\0';
    write(sockfd,buffer,sizeof(buffer));
    cout << "aaaa" << endl;
}
