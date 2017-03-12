void put (argumentBox* box, unsigned size, socket_t* tube) {
    bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.m["Command"] = "PUT";
    message.m["Remote-Path"] = box->remote_path;
    message.m["Type"] = "file";
    message.fileInit(box->local_path.c_str(), "rb");

    do {
        is_full = message.writeRequest(buffer, size);
        if (send(tube->client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
    } while (is_full);

    message.fileClose();

    message.parseRequest(buffer, size);
}

void mkd (argumentBox* box, unsigned size, socket_t* tube) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.m["Command"] = "PUT";
    message.m["Remote-Path"] = box->remote_path;
    message.m["Type"] = "folder";

    message.writeRequest(buffer, size);
    if (send(tube->client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void get (argumentBox* box, unsigned size, socket_t* tube) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.m["Command"] = "GET";
    message.m["Remote-Path"] = box->remote_path;
    message.m["Type"] = "file";
    message.fileInit(box->local_path.c_str(), "wb");

    message.writeRequest(buffer, size);
    if (send(tube->client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void lst (argumentBox* box, unsigned size, socket_t* tube) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.m["Command"] = "GET";
    message.m["Remote-Path"] = box->remote_path;
    message.m["Type"] = "folder";

    message.writeRequest(buffer, size);
    if (send(tube->client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void del (argumentBox* box, unsigned size, socket_t* tube) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.m["Command"] = "DEL";
    message.m["Remote-Path"] = box->remote_path;
    message.m["Type"] = "file";

    message.writeRequest(buffer, size);
    if (send(tube->client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void rmd (argumentBox* box, unsigned size, socket_t* tube) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.m["Command"] = "DEL";
    message.m["Remote-Path"] = box->remote_path;
    message.m["Type"] = "folder";

    message.writeRequest(buffer, size);
    if (send(tube->client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}
