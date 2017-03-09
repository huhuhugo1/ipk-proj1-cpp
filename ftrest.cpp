/*
 * IPK.2015L
 *
 * Demonstration of trivial TCP communication.
 *
 * Ondrej Rysavy (rysavy@fit.vutbr.cz)
 *
 */
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFSIZE 1024

struct Socket {
    int client_socket;
    public:
        Socket(const char* server_hostname, int port_number) {
            struct hostent *server;
            if ((server = gethostbyname(server_hostname)) == NULL) {
                fprintf(stderr,"ERROR: no such host as %s\n", server_hostname);
                exit(EXIT_FAILURE);
            }

            struct sockaddr_in server_address;
            bzero((char *) &server_address, sizeof(server_address));
            server_address.sin_family = AF_INET;
            bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
            server_address.sin_port = htons(port_number);

            if ((this->client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
                perror("ERROR: socket");
                exit(EXIT_FAILURE);
            }

            if (connect(this->client_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0) {
            perror("ERROR: connect");
            exit(EXIT_FAILURE);        
            }
        }
        ~Socket() {
            close(this->client_socket);
        }
};
struct argumentBox {
    std::string command;
    std::string host;
    std::string port;
    std::string path;

    argumentBox(int argc, const char * argv[]) {
        std::string url;
        if (argc == 3) {
            if (strlen(argv[2]) >= 7)
                url.assign(argv[2]+7);

            this->command.assign(argv[1]);

            int semicolon_idx = url.find(':', 0);
            int path_start_idx = url.find('/', semicolon_idx);
            
            this->host.assign(url.substr(0, semicolon_idx));
            this->port.assign(url.substr(semicolon_idx + 1, path_start_idx - semicolon_idx - 1));
            this->path.assign(url.substr(path_start_idx, -1));
        }
    }

    void print() {
        printf("command: %s\nport: %s\nhost: %s\npath: %s\n", command.c_str(), port.c_str(), host.c_str(), path.c_str());
    }
};



int main (int argc, const char * argv[]) {
    
    argumentBox box(argc, argv);
    box.print();
    
    return 0;
    
    Socket socket(box.host.c_str(), atoi(box.port.c_str()));


    char buf[BUFSIZE];

    int bytestx, bytesrx;    

    /* nacteni zpravy od uzivatele */
    bzero(buf, BUFSIZE);
    printf("Please enter msg: ");
    fgets(buf, BUFSIZE, stdin);
    
    

    /* odeslani zpravy na server */
    bytestx = send(socket.client_socket, buf, strlen(buf), 0);
    if (bytestx < 0) 
      perror("ERROR in sendto");
    
    /* prijeti odpovedi a jeji vypsani */
    bytesrx = recv(socket.client_socket, buf, BUFSIZE, 0);
    if (bytesrx < 0) 
      perror("ERROR in recvfrom");
      
    printf("Echo from server: %s", buf);
        
    
    return 0;
}
