#include <iostream>
#include <string.h>
#include <sys/socket.h>
//#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

struct socket_t {
    int client_socket;

    socket_t(const char* server_hostname, int port_number) {
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
    ~socket_t() {
        close(this->client_socket);
    }
};
