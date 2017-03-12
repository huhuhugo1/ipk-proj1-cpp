#include <iostream>
#include <string.h>

using namespace std;

struct HTTP_message_t {
    string command;
    string path;
    string type;
    string accept;
    string encoding;
    string con_type;
    unsigned long long length;

    FILE* file;

    HTTP_message_t() {
        encoding = "identify";
        accept = "application/octet-stream";
        con_type = "application/octet-stream";
        file = NULL;
    }

    ~HTTP_message_t() {
        fclose(file);
    }

    char* getDate() {
        static char date[100];
        time_t cas = time(NULL);
        strftime(date, 100, "%a, %d %b %Y %X CET", localtime(&cas));
        return date;
    }

    void fileInit(const char* name, const char* mode) {
        file = fopen(name, mode);
        if (file) {
            fseek(file, 0L, SEEK_END);
            length = ftell(file);
            rewind(file);
        }
    } 

    bool writeRequest (char* buffer, size_t size) {
        memset(buffer, 0, size);
        
        sprintf(buffer,
        "%s %s?type=%s HTTP/1.1\r\n"
        "Date: %s\r\n"
        "Accept: %s\r\n"
        "Accept-Encoding: %s\r\n",
        command.c_str(), path.c_str(), type.c_str(), getDate(), 
        accept.c_str(), encoding.c_str());
        
        if (command != "DEL")
            sprintf(buffer + strlen(buffer),
            "Content-Type: %s\r\n"
            "Content-Length: %llu\r\n\r\n",
            con_type.c_str(), length);
        else
            sprintf(buffer + strlen(buffer), "\r\n");

        if (file) {
            int body_offset = strlen(buffer);
            int body_length = size - body_offset;

            if (body_length == fread(buffer + body_offset, sizeof(char), body_length, file))
                return true;
        }
        return false;
    }

    int parseResponseHead (char* buffer, size_t size) {
        ;
    }
};
