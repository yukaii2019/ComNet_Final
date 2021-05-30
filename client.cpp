#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>

using namespace std;

void Server_Request_Procedure(void);
void Send(string str);
void Recieve(void);

int sockfd;
char recieve_buffer[100];

int main(){
    struct sockaddr_in server_addr;
    int status;
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    
    sockfd = socket(PF_INET,SOCK_STREAM,0);
    int c = connect(sockfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in));
    if (c == -1){
        cout << "connect Error !!" << endl;
        return 1;
    }
    else {
        cout << "Successfully connect to server" << endl;
    }
    Server_Request_Procedure();
    close(sockfd);
    return 0;
}

void Server_Request_Procedure(){
    string input;
    while(1){
        Recieve();
        cout << recieve_buffer << " ";
        while(1){
            cin >> input;
            Send(input);
            Recieve();
            if(strcmp(recieve_buffer,"correct")==0){
                break;
            }
            else{
                cout << recieve_buffer << " ";
            }
        }

        if(input == "1"){
            Recieve();
            cout << recieve_buffer << " ";
            cin >> input;
            Send(input);
            Recieve();
            if(strcmp(recieve_buffer,"error")==0){
                cout << "Can't find the address" << endl<<endl;
            }
            else{
                cout << "address get from domain name : "<<recieve_buffer << endl << endl;
            }
        }
        else if(input =="2"){
            Recieve();
            cout << recieve_buffer << " ";
            cin >> input;
            Send(input);
            Recieve();
            cout << "Email get from server : "<< recieve_buffer << endl << endl;
        }
        else{
            break;
        }
    }
}
void Send(string str){
    char send_buffer[100];
    strcpy(send_buffer,str.c_str());
    write(sockfd,send_buffer,sizeof(char)*strlen(send_buffer));
    usleep(1000);
}

void Recieve(){
    memset(recieve_buffer,0,sizeof(recieve_buffer));
    read(sockfd,recieve_buffer,sizeof(recieve_buffer));
    usleep(1000);
}
