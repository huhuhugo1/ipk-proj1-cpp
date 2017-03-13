void putRequest (argumentBox* box, unsigned size, int client_socket) {
    FILE* file = fopen(box->local_path.c_str(), "rb");
    struct HTTP_message_t message;
    char buffer[size];

    fseek(file, 0L, SEEK_END);
    unsigned long long expected_length = ftell(file);
    rewind(file);
    unsigned long long read = 0;

    message["Command"] = "PUT";
    message["Remote-Path"] = box->remote_path;
    message["Type"] = "file";
    message["Content-Length"] = to_string(expected_length);

    message.writeRequestHead(buffer, size);

    char* body_pos = buffer + strlen(buffer);

    while (read < expected_length) {
        int read_length = size;                         //velkost celeho bufferu
        if (body_pos - buffer + read_length > size)     //ak by to precilo velkost velkost celeho buferu
            read_length = size - (body_pos - buffer);   //tak tam zapis iba zvysok

        read += fread(body_pos, sizeof(char), read_length, file);
        body_pos = buffer;

        switch (send(client_socket, buffer, size, 0)) {
            case -1: break;
            case  0: break;
            default: break;
        }
    };
    fclose(file);
}

void mkd (argumentBox* box, unsigned size, int client_socket) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message["Command"] = "PUT";
    message["Remote-Path"] = box->remote_path;
    message["Type"] = "folder";

    message.writeRequestHead(buffer, size);
    if (send(client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void get (argumentBox* box, unsigned size, int client_socket) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message["Command"] = "GET";
    message["Remote-Path"] = box->remote_path;
    message["Type"] = "file";
    //message.fileInit(box->local_path.c_str(), "wb");

    message.writeRequestHead(buffer, size);
    if (send(client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void lst (argumentBox* box, unsigned size, int client_socket) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message["Command"] = "GET";
    message["Remote-Path"] = box->remote_path;
    message["Type"] = "folder";

    message.writeRequestHead(buffer, size);
    if (send(client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");

    while (recv(client_socket, buffer, size, 0) > 0)
        printf("%s", buffer);
    
}

void del (argumentBox* box, unsigned size, int client_socket) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message["Command"] = "DEL";
    message["Remote-Path"] = box->remote_path;
    message["Type"] = "file";

    message.writeRequestHead(buffer, size);
    if (send(client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void rmd (argumentBox* box, unsigned size, int client_socket) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message["Command"] = "DEL";
    message["Remote-Path"] = box->remote_path;
    message["Type"] = "folder";

    message.writeRequestHead(buffer, size);
    if (send(client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

