#include <iostream>
#include <string.h>
#include <sys/socket.h>
//#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <regex>

#define BUFSIZE 1024
using namespace std;

char* getDate() {
    static char date[1000];
    time_t cas = time(NULL);
    strftime(date, 1000, "%a, %d %b %Y %X CET", localtime(&cas));
    return date;
}

struct HTTP_head_t {
    string command;
    string path;
    string type;
    string host;
    string date;
    string accept;
    string encoding;
    string con_type; 
    unsigned long long length;
};

void write_HTTP_REQ_HEAD (char* buffer, HTTP_head_t* head) {
    *buffer = {0};
    sprintf(buffer,
    "%s %s?type=%s HTTP/1.1\r\n"
    "Host: %s\r\n"
    "Date: %s\r\n"
    "Accept: %s\r\n"
    "Accept-Encoding: %s\r\n"
    "Content-Type: %s\r\n"
    "Content-Length: %llu\r\n",
    head->command.c_str(), head->path.c_str(), head->type.c_str(), head->host.c_str(), getDate(), 
    head->accept.c_str(), head->encoding.c_str(), head->con_type.c_str(), head->length);
};

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
    string command;
    string host;
    unsigned port;
    string path;

    argumentBox(int argc, const char * argv[]) {
        if (argc != 3) {
            perror("Invalid number of parameters.");
            exit(1);
        }
        string url(argv[2]);

        if (regex_match(url, regex("^http:\\/\\/[^:]+:[0-9]+\\/.*$"))) {
            url.assign(argv[2]+7);
            this->command.assign(argv[1]);

            int semicolon_idx = url.find(':', 0);
            int path_start_idx = url.find('/', semicolon_idx);
            
            this->host.assign(url.substr(0, semicolon_idx));
            this->port = atoi(url.substr(semicolon_idx + 1, path_start_idx - semicolon_idx - 1).c_str());
            this->path.assign(url.substr(path_start_idx, -1));
            return;
        } else {
            perror("Invalid format of url.");
            exit(1);
        }
    }
};

int main (int argc, const char * argv[]) {
    char buffer[BUFSIZE];
    struct HTTP_head_t head;

    argumentBox box(argc, argv);
    Socket tube(box.host.c_str(), box.port); 
    
    FILE* file = fopen(box.path.c_str(), "rb");
    head.command = "GET";
    //head.

    int writen;
    do {
        write_HTTP_REQ_HEAD(buffer, &head);    
        writen = fread(buffer + strlen(buffer), sizeof(char), BUFSIZE - strlen(buffer), file);
        
        if (send(tube.client_socket, buffer, strlen(buffer), 0) < 0) 
            perror("ERROR in sendto");

        if (recv(tube.client_socket, buffer, BUFSIZE, 0) < 0) 
            perror("ERROR in recvfrom");
    } while (writen > BUFSIZE - strlen(buffer));

    return 0;
}
