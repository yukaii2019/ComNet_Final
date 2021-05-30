#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <fstream>
#include <vector>
#include <netinet/in.h>
#include <netdb.h>
using namespace std;

typedef struct ID_Email_Pair {
    string ID;
    string Email;
}ID_Email_Pair;

int handle_client(void);
int URL_to_Address(char* URL, char* ip);
void Send(string str);
void Recieve(void);
vector<ID_Email_Pair> ID_Email;

int streamfd;
char recieve_buffer[100];

int main(){
    struct sockaddr_in myaddr;
    struct sockaddr_in client_addr;
    int sockfd, port, status;
    int addr_size; 

/*  read query.txt-----------------------------------------*/
    ifstream infile;
    infile.open("query.txt",ios::in);
    if(!infile){
        cout << "Can't not open query.txt"<< endl;
        return 1;
    }
    else{
        cout << "Open query.txt success" << endl;
    }
    while(!infile.eof()){
        ID_Email_Pair tmp;
        infile >> tmp.ID;
        infile >> tmp.Email;
        ID_Email.push_back(tmp);
    }
    infile.close();
/*end of reading query.txt---------------------------------------*/

    bzero(&myaddr,sizeof(myaddr));
    myaddr.sin_family = PF_INET;
    myaddr.sin_port=htons(1234);
    myaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    sockfd = socket(PF_INET,SOCK_STREAM,0);
    bind(sockfd,(struct sockaddr*) &myaddr,sizeof(struct sockaddr_in));
    listen(sockfd,10);
    addr_size = sizeof(client_addr);
    while(1){
        cout << "Waiting for connection..." << endl;
        streamfd = accept(sockfd,(struct sockaddr*) &client_addr,(socklen_t*)&addr_size);
        if (streamfd == -1){
            cout << "Accept Error !!" << endl;
            return 1;
        }
        else{
            cout << "New connection from " << ntohl(client_addr.sin_addr.s_addr)/16777216 << "." << (ntohl(client_addr.sin_addr.s_addr)%16777216)/65536 << '.' << (ntohl(client_addr.sin_addr.s_addr)%65536)/256  << "." << (ntohl(client_addr.sin_addr.s_addr)%256)<< endl;
        }
        handle_client();

        cout << "Disconnect from " << ntohl(client_addr.sin_addr.s_addr)/16777216 << "." << (ntohl(client_addr.sin_addr.s_addr)%16777216)/65536 << '.' << (ntohl(client_addr.sin_addr.s_addr)%65536)/256  << "." << (ntohl(client_addr.sin_addr.s_addr)%256)<< endl;

        close(streamfd);
    }
    return 0;
}

int handle_client(){

    char IP[100];
    while(1){
        /*read requirement---------------------------------------------*/
        Send("What's your requirement? 1.DNS 2.QUERY 3.QUIT :");
        while(1){ 
            Recieve();
            if(strcmp(recieve_buffer,"1")==0||strcmp(recieve_buffer,"2")==0|| strcmp(recieve_buffer,"3")==0){
                cout << "Requirement from client is " << recieve_buffer << endl;
                Send("correct");
                break;
            }
            else{
                cout << "The requirment from client isn't exit" << endl;
                Send("The requirement isn't exist, please enter 1, 2 or 3\n\nWhat's your requirement? 1.DNS 2.QUERY 3.QUIT :");
            } 
        }
        /*-------------------------------------------------------------*/

        if(strcmp(recieve_buffer,"1")==0){
            Send("Input URL address :");
            Recieve();
            cout << "The recieved URL is " << recieve_buffer << endl;
            cout << "Finding..." << endl;
            int u = URL_to_Address(recieve_buffer,IP);
            if(u==1){
                cout << "Can't find the IP address" << endl;
                Send("error");
            }
            else{
                cout << "The IP address sent back is " << string(IP) << endl;
                Send(string(IP));
            }
        }
        else if (strcmp(recieve_buffer,"2")==0){
            Send("Input student ID :");
            Recieve();
            cout << "The recieved student ID is "<< recieve_buffer << endl;
            bool find = false;
            for(int i=0;i<ID_Email.size();i++){
                if(ID_Email[i].ID==string(recieve_buffer)){
                    find = true;
                    cout << "The Email send back is " << ID_Email[i].Email << endl;
                    Send(ID_Email[i].Email);
                    break;
                }
            }
            if(find == false){
                cout << "Can't find the student" << endl;
                Send("No such student ID");
            }
        }
        else {
            break;
        }
    }
    return 0;
}


int URL_to_Address(char* URL, char* ip){
    struct hostent* he;
    struct in_addr** addrList;
    if((he = gethostbyname(URL))==NULL){
        return 1;
    }

    addrList =(struct in_addr**) he->h_addr_list;

    for(int i = 0;addrList!=NULL;i++){
        strcpy(ip,inet_ntoa(*addrList[i]));
        return 0;
    }
    
    return 1;
}

void Send(string str){
    char send_buffer[100];
    strcpy(send_buffer,str.c_str());
    write(streamfd,send_buffer,sizeof(char)*strlen(send_buffer));
    usleep(1000);
}

void Recieve(){
    memset(recieve_buffer,0,sizeof(recieve_buffer));
    read(streamfd,recieve_buffer,sizeof(recieve_buffer));
    usleep(1000);
}
