#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include <iostream>
#include"common/message.cpp"

#define BUFSIZE 1024
using namespace std;
 
int main(int argc, char *argv[]) {
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[BUFSIZE];
    
    if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Could not create socket");
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    if( bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }

    listen(socket_desc, 3);
    
    c = sizeof(struct sockaddr_in);
    
    while (true) {
        if ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) < 0) {
            perror("accept failed");
            return 1;
        }
        HTTP_message_t message;
        while ((read_size = recv(client_sock, client_message, BUFSIZE, 0)) > 0 ) {
            //Send the message back to client
            //write(client_sock, client_message, strlen(client_message));
            message.parseRequest(client_message, BUFSIZE);
        }
        fflush(message.file);
        message.fileClose();
        
        if (read_size == 0) {
            puts("Client disconnected");
            fflush(stdout);
        } else if(read_size == -1) {
            perror("recv failed");
        }
    }

    return 0;
}
