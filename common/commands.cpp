int putRequest (argumentBox* box, unsigned size, int client_socket) {
    FILE* file;
    if ((file = fopen(box->local_path.c_str(), "rb")) == NULL) {
        fprintf(stderr, "Unknown error.\n");
        return 1;
    }

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
    }

    fclose(file);
    
    switch (recv(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }

    message.parseResponseHead(buffer, size);
    
    if (message["Code"] != "200 OK") {
        char* body_pos = strstr(buffer, "\r\n\r\n") + 4;
        fprintf(stderr, "%.*s", size, body_pos);
        return 1;
    }
    return 0;
}

int lstRequest (argumentBox* box, unsigned size, int client_socket) {
    char buffer[size];
    struct HTTP_message_t message;

    message["Command"] = "GET";
    message["Remote-Path"] = box->remote_path;
    message["Type"] = "folder";

    message.writeRequestHead(buffer, size);

    switch (send(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }

    switch (recv(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }

    message.parseResponseHead(buffer, size);

    if (message["Code"] != "200 OK") {
        char* body_pos = strstr(buffer, "\r\n\r\n") + 4;
        fprintf(stderr, "%.*s", size, body_pos);
        return 1;
    }

    char* body_pos = strstr(buffer, "\r\n\r\n") + 4;
    printf("%.*s", size, body_pos);
    
    while (buffer[size - 1] != 0) {
        switch (recv(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
        }
        printf("%.*s", size, buffer);
    } 
    return 0;
}

int mkdRequest (argumentBox* box, unsigned size, int client_socket) {
    char buffer[size];
    struct HTTP_message_t message;

    message["Command"] = "PUT";
    message["Remote-Path"] = box->remote_path;
    message["Type"] = "folder";

    message.writeRequestHead(buffer, size);

    switch (send(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }

    switch (recv(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }

    message.parseResponseHead(buffer, size);
    
    if (message["Code"] != "200 OK") {
        char* body_pos = strstr(buffer, "\r\n\r\n") + 4;
        fprintf(stderr, "%.*s", size, body_pos);
        return 1;
    }
    return 0;
}

int rmdRequest (argumentBox* box, unsigned size, int client_socket) {
    char buffer[size];
    struct HTTP_message_t message;

    message["Command"] = "DEL";
    message["Remote-Path"] = box->remote_path;
    message["Type"] = "folder";

    message.writeRequestHead(buffer, size);

    switch (send(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }

    switch (recv(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }

    message.parseResponseHead(buffer, size);
    
    if (message["Code"] != "200 OK") {
        char* body_pos = strstr(buffer, "\r\n\r\n") + 4;
        fprintf(stderr, "%.*s", size, body_pos);
        return 1;
    }
    return 0;
}

int delRequest (argumentBox* box, unsigned size, int client_socket) {
    char buffer[size];
    struct HTTP_message_t message;

    message["Command"] = "DEL";
    message["Remote-Path"] = box->remote_path;
    message["Type"] = "file";

    message.writeRequestHead(buffer, size);

    switch (send(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }

    switch (recv(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }

    message.parseResponseHead(buffer, size);
    
    if (message["Code"] != "200 OK") {
        char* body_pos = strstr(buffer, "\r\n\r\n") + 4;
        fprintf(stderr, "%.*s", size, body_pos);
        return 1;
    }
    return 0;
}

int getRequest (argumentBox* box, unsigned size, int client_socket) {
    char buffer[size];
    struct HTTP_message_t message;

    message["Command"] = "GET";
    message["Remote-Path"] = box->remote_path;
    message["Type"] = "file";

    message.writeRequestHead(buffer, size);
    
    switch (send(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }

    switch (recv(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }

    message.parseResponseHead(buffer, size);
    
    if (message["Code"] != "200 OK") {
        char* body_pos = strstr(buffer, "\r\n\r\n") + 4;
        fprintf(stderr, "%.*s", size, body_pos);
        return 1;
    }

    FILE* file = fopen(box->local_path.c_str(), "wb");
    unsigned long long expected_size = stoull(message["Content-Length"]);
    unsigned long long written = 0;
    char* body_pos = strstr(buffer, "\r\n\r\n") + 4;

    while (true) {
        int write_length = size - (body_pos - buffer);  //velkost celeho tela spravy
        if (write_length + written > expected_size)     //ak by to precilo velkost suboru
            write_length = expected_size - written;     //tak tam zapis iba zvysok
        
        written += fwrite(body_pos, sizeof(char), write_length, file);
        body_pos = buffer;
        
        if (written < expected_size) {
            switch (recv(client_socket, buffer, size, 0)) {
                case -1: break;
                case  0: break;
                default: break;
            }
        } else
            break;
        
    }

    fclose(file); 
    return 0;
}

