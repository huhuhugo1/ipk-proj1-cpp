void put (argumentBox* box, unsigned size, socket_t* tube) {
    bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.command = "PUT";
    message.path = box->remote_path;
    message.type = "file";
    message.fileInit(box->local_path.c_str(), "rb");

    do {
        is_full = message.writeRequest(buffer, size);
        if (send(tube->client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
    } while (is_full);


}

void mkd (argumentBox* box, unsigned size, socket_t* tube) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.command = "PUT";
    message.path = box->remote_path;
    message.type = "folder";

    message.writeRequest(buffer, size);
    if (send(tube->client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void get (argumentBox* box, unsigned size, socket_t* tube) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.command = "GET";
    message.path = box->remote_path;
    message.type = "file";
    message.fileInit(box->local_path.c_str(), "wb");

    message.writeRequest(buffer, size);
    if (send(tube->client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void lst (argumentBox* box, unsigned size, socket_t* tube) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.command = "GET";
    message.path = box->remote_path;
    message.type = "folder";

    message.writeRequest(buffer, size);
    if (send(tube->client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void del (argumentBox* box, unsigned size, socket_t* tube) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.command = "DEL";
    message.path = box->remote_path;
    message.type = "file";

    message.writeRequest(buffer, size);
    if (send(tube->client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}

void rmd (argumentBox* box, unsigned size, socket_t* tube) {
    //bool is_full;
    char buffer[size];
    struct HTTP_message_t message;

    message.command = "DEL";
    message.path = box->remote_path;
    message.type = "folder";

    message.writeRequest(buffer, size);
    if (send(tube->client_socket, buffer, size, 0) < 0)
            perror("ERROR in sendto");
}
