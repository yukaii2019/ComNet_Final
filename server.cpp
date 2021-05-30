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

int handle_client(void);
int URL_to_Address(char* URL, char* ip);
void Send(string str);
void Recieve(void);

int streamfd;
char recieve_buffer[100];

typedef struct ID_Email_Pair {
    string ID;
    string Email;
}ID_Email_Pair;


int main(){
    struct sockaddr_in myaddr;
    struct sockaddr_in client_addr;
    int sockfd, port, status;
    //int streamfd;
    int addr_size;
     
    vector<ID_Email_Pair> ID_Email;
    
    //char URL[] = "www.google.com";
    //URL_to_Address(URL,IP_buffer);
    //cout << IP_buffer << endl;

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
    /*
    for(int i = 0 ;i< ID_Email.size();i++){
        cout << ID_Email[i].ID << endl;
        cout << ID_Email[i].Email << endl;
        
    }
     */
    infile.close();
/*end of read query.txt---------------------------------------*/


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
        if (streamfd == -1){
            cout << "Accept Error !!" << endl;
            return 1;
        }  
        handle_client();
        close(streamfd);
    }
    return 0;
}

int handle_client(){
    char str_buffer2[] = "Input URL address : ";
    char str_buffer3[] = "Input student ID : ";
    
    /*read requirement---------------------------------------------*/
    while(1){ 
        Send("What's your requirement? 1.DNS 2.QUERY 3.QUIT");
        Recieve();
        if(strcmp(recieve_buffer,"1")==0||strcmp(recieve_buffer,"2")==0|| strcmp(recieve_buffer,"3")==0){
            Send("correct");
            break;
        }
        else{
            Send("The requirement isn't exist, please enter 1, 2 or 3\n");
        } 
    }
    /*-------------------------------------------------------------*/
    cout << "aaaaaaaaaaaaaaaaaaaaa"<<endl;
    if(strcmp(recieve_buffer,"1")==0){

    }
    else if (strcmp(recieve_buffer,"2")==0){

    }
    else {

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
}

void Recieve(){
    memset(recieve_buffer,0,sizeof(recieve_buffer));
    read(streamfd,recieve_buffer,sizeof(recieve_buffer));
}
