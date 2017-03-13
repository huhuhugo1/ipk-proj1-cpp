#include <iostream>
#include <string.h>
#include <unordered_map>
#include <sstream>

using namespace std;

struct HTTP_message_t {
    unordered_map<string, string> m;
    FILE* file;

    HTTP_message_t() {
        m["Accept-Encoding"] = "identify";
        m["Accept"] = "application/octet-stream";
        m["Content-Type"] = "application/octet-stream";
        file = NULL;
    }

    void setDate() {
        static char date[100];
        time_t cas = time(NULL);
        strftime(date, 100, "%a, %d %b %Y %X CET", localtime(&cas));
        m["Date"] = string(date);
    }

    void fileInit(const char* name, const char* mode) {
        file = fopen(name, mode);
        if (file) {
            fseek(file, 0L, SEEK_END);
            m["Content-Length"] = to_string(ftell(file));
            rewind(file);
        } else
            printf("error file");
    } 

    void fileClose() {
        if (file)
            fclose(file);
    }

    bool writeRequest (char* buffer, size_t size) {
        bzero(buffer, size);
        setDate();

        sprintf(buffer,
        "%s %s?type=%s HTTP/1.1\r\n"
        "Date: %s\r\n"
        "Accept: %s\r\n"
        "Accept-Encoding: %s\r\n",
        m["Command"].c_str(), m["Remote-Path"].c_str(), m["Type"].c_str(), m["Date"].c_str(), 
        m["Accept"].c_str(), m["Accept-Encoding"].c_str());
        
        if (m["Command"] != "DEL")
            sprintf(buffer + strlen(buffer),
            "Content-Type: %s\r\n"
            "Content-Length: %s\r\n\r\n",
            m["Content-Type"].c_str(), m["Content-Length"].c_str());
        else
            sprintf(buffer + strlen(buffer), "\r\n");

        if (file) {
            int body_length = size - strlen(buffer);
            if (body_length == fread(buffer + strlen(buffer), sizeof(char), body_length, file))
                return true;
        }

        return false;
    }

    int parseRequest (char* buffer, size_t size) {
        char* body_pos = strstr(buffer, "\r\n\r\n") + 4;
        string head(buffer, body_pos - buffer);
        stringstream atributes(head.substr(head.find("\r\n") + 2));
        string line;

        m["Command"] = head.substr(0, head.find(" "));
        m["Remote-Path"] = head.substr(head.find(" ") + 1, head.find("?type=") - head.find(" ") - 1);
        m["Type"] = head.substr(head.find("?type=") + 6, head.find(" ", head.find("?type=")) - head.find("?type=") - 6);

        while (getline(atributes, line)) {
            string index = line.substr(0, line.find(":"));
            string element = line.substr(line.find(":") + 2);
            m[index] = element;
        }

        if (m["Command"] == "PUT") {
            int filesize = atoi(m["Content-Length"].c_str());
            if (!file)
                file = fopen(("." + m["Remote-Path"]).c_str(), "wb");
            
            int body_length = size - (body_pos - buffer);
            if (body_length > filesize)
                body_length = filesize;
            
            if (filesize -= fwrite(body_pos, sizeof(char), body_length, file))
                return true;
        }

    }
};
