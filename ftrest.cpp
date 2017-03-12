#include <iostream>
#include <string.h>
#include <sys/socket.h>
//#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <regex>

#include "box.cpp"
#include "message.cpp"
#include "socket.cpp"
#include "commands.cpp"

#define BUFSIZE 1024
using namespace std;

int main (int argc, const char * argv[]) {
    argumentBox box;
    box.load(argc, argv);
    socket_t tube(box.host.c_str(), box.port);
    
    if (box.command == "put")
        put(&box, BUFSIZE, &tube);
    else if (box.command == "mkd")
        mkd(&box, BUFSIZE, &tube);
    else if (box.command == "get")
        get(&box, BUFSIZE, &tube);
    else if (box.command == "lst")
        lst(&box, BUFSIZE, &tube);
    else if (box.command == "del")
        del(&box, BUFSIZE, &tube);
    else if (box.command == "rmd")
        rmd(&box, BUFSIZE, &tube);
    else
        exit(1);//todo error

    return 0;
}
