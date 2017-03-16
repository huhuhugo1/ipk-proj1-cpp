#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include <iostream>
#include "common/message.cpp"
#include "common/responses.cpp"

#define BUFSIZE 4096
using namespace std;

int main(int argc, char *argv[]) {
    string root_path = ".";
    int port = 6677;

    //Arguments process
    switch (argc) {
        case 1:            
            break;
        case 5:
            if (string(argv[3]) == "-r")
                root_path = string(argv[4]);
            else if (string(argv[3]) == "-p")
                port = atoi(argv[4]);
            else
                ;//error;
        case 3:
            if (string(argv[1]) == "-r")
                root_path = string(argv[2]);
            else if (string(argv[1]) == "-p")
                port = atoi(argv[2]);
            else
                ;//error;
    }
        if (root_path.back() == '/')
            root_path.pop_back();

    cout << port << endl;
    cout << root_path << endl;

    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char buffer[BUFSIZE];
    
    if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Could not create socket");
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
     
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
        if (read_size == 0) {
            puts("Client disconnected");
            fflush(stdout);
        } else if(read_size == -1) {
            perror("recv failed");
        }

        HTTP_message_t message;
        message.parseRequestHead(buffer, BUFSIZE);

        if (message["Command"] == "PUT" && message["Type"] == "file")
            putResponse(client_sock, buffer, BUFSIZE, message, root_path);
        
        else if (message["Command"] == "PUT" && message["Type"] == "folder")
            mkdResponse(client_sock, buffer, BUFSIZE, message, root_path);
            
        else if (message["Command"] == "GET" && message["Type"] == "file")
            getResponse(client_sock, buffer, BUFSIZE, message, root_path);
        
        else if (message["Command"] == "GET" && message["Type"] == "folder")
            lstResponse(client_sock, buffer, BUFSIZE, message, root_path);
        
        else if (message["Command"] == "DEL" && message["Type"] == "file")
            delResponse(client_sock, buffer, BUFSIZE, message, root_path);
        
        else if (message["Command"] == "DEL" && message["Type"] == "folder")
            rmdResponse(client_sock, buffer, BUFSIZE, message, root_path);
        
        else
            printf("Unknown command\n");//TODO errorssssssss
    }

    return 0;
}
