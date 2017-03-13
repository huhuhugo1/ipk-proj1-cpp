#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include <iostream>
#include "common/message.cpp"
#include "common/responses.cpp"

#define BUFSIZE 1024
using namespace std;
 
int main(int argc, char *argv[]) {
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char buffer[BUFSIZE];
    
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

        read_size = recv(client_sock, buffer, BUFSIZE, 0);
        HTTP_message_t message;
        message.parseRequestHead(buffer, BUFSIZE);

        if (message["Command"] == "PUT" && message["Type"] == "file")
            read_size = putResponse(client_sock, buffer, BUFSIZE, message);
        
        else if (message["Command"] == "PUT" && message["Type"] == "folder")
            ;//mkdResponse(client_sock, buffer, BUFSIZE, message);
        
        else if (message["Command"] == "GET" && message["Type"] == "file")
            ;//getReaction(client_sock, buffer, BUFSIZE, message);
        
        else if (message["Command"] == "GET" && message["Type"] == "folder")
            read_size = lstResponse(client_sock, buffer, BUFSIZE, message);
        
        else if (message["Command"] == "DEL" && message["Type"] == "file")
            ;//delReaction(client_sock, buffer, BUFSIZE, message);
        
        else if (message["Command"] == "DEL" && message["Type"] == "folder")
            ;//rmdReaction(client_sock, buffer, BUFSIZE, message);
        
        else
            ;//TODO error

        if (read_size == 0) {
            puts("Client disconnected");
            fflush(stdout);
        } else if(read_size == -1) {
            perror("recv failed");
        }
    }

    return 0;
}
