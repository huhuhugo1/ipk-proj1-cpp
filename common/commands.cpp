#include "box.cpp"
void put (argumentBox* box, unsigned size, int client_socket) {
    bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.m["Command"] = "PUT";
    message.m["Remote-Path"] = box->remote_path;
    message.m["Type"] = "file";
    message.fileInit(box->local_path.c_str(), "rb");

    do {
        is_full = message.writeRequest(buffer, size);
        if (send(client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
    } while (is_full);

    message.fileClose();
}

int putReaction(int client_socket, char* buffer, int size, string path, unsigned long long filesize) {
    int read_size;
    char* body_pos = strstr(buffer, "\r\n\r\n") + 4;
    int body_length = size - (body_pos - buffer);
    FILE* file = fopen(("." + path).c_str(), "wb");
    
    do {
        if (body_length > filesize)
            body_length = filesize;
        
        filesize -= fwrite(body_pos, sizeof(char), body_length, file);
    } while ((read_size = recv(client_socket, buffer, size, 0)) > 0 );

        fflush(file);
        fclose(file); 
    return read_size;
}

void mkd (argumentBox* box, unsigned size, int client_socket) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.m["Command"] = "PUT";
    message.m["Remote-Path"] = box->remote_path;
    message.m["Type"] = "folder";

    message.writeRequest(buffer, size);
    if (send(client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void get (argumentBox* box, unsigned size, int client_socket) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.m["Command"] = "GET";
    message.m["Remote-Path"] = box->remote_path;
    message.m["Type"] = "file";
    message.fileInit(box->local_path.c_str(), "wb");

    message.writeRequest(buffer, size);
    if (send(client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void lst (argumentBox* box, unsigned size, int client_socket) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.m["Command"] = "GET";
    message.m["Remote-Path"] = box->remote_path;
    message.m["Type"] = "folder";

    message.writeRequest(buffer, size);
    if (send(client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void del (argumentBox* box, unsigned size, int client_socket) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.m["Command"] = "DEL";
    message.m["Remote-Path"] = box->remote_path;
    message.m["Type"] = "file";

    message.writeRequest(buffer, size);
    if (send(client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void rmd (argumentBox* box, unsigned size, int client_socket) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.m["Command"] = "DEL";
    message.m["Remote-Path"] = box->remote_path;
    message.m["Type"] = "folder";

    message.writeRequest(buffer, size);
    if (send(client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}
