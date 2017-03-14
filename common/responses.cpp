#include <dirent.h>
#include <sys/stat.h>

int putResponse(int client_socket, char* buffer, int size, HTTP_message_t message, string root_path) {
    FILE* file = fopen((root_path + message["Remote-Path"]).c_str(), "wb");
    unsigned long long expected_size = stoull(message["Content-Length"]);
    unsigned long long written = 0;
    char* body_pos = strstr(buffer, "\r\n\r\n") + 4;
    
    while (true) {
        int write_length = size - (body_pos - buffer);  //velkost celeho tela spravy
        if (write_length + written > expected_size)     //ak by to precilo velkost suboru
            write_length = expected_size - written;     //tak tam zapis iba zvysok
        
        written += fwrite(body_pos, sizeof(char), write_length, file);
        body_pos = buffer;
        
        if (written < expected_size)
            switch (recv(client_socket, buffer, size, 0)) {
                case -1: break;
                case  0: break;
                default: break;
            }
        else
            break;
    }

    fclose(file); 
    return 0;//read_size;
}

int lstResponse(int client_socket, char* buffer, int size, HTTP_message_t message, string root_path) {
    string folder_content;
    struct stat statbuf;

    if (stat((root_path + message["Remote-Path"]).c_str(), &statbuf) != 0)
        message["Code"] = "404 Not Found";   
    
    else if (!S_ISDIR(statbuf.st_mode))
        message["Code"] = "400 Bad Request";
    
    else {
        if (DIR *dpdf = opendir((root_path + message["Remote-Path"]).c_str()))
            while (struct dirent *epdf = readdir(dpdf))
                folder_content.append(epdf->d_name).append("\n");
        else 
            ;//todo 
        message["Code"] = "200 OK";
        message["Content-Length"] = to_string(folder_content.length());
    }

    message["Content-Encoding"] = "identity"; 
    message["Content-Type"] = "text/plain";
    
    message.writeResponseHead(buffer, size);

    folder_content.insert(0, buffer);

    unsigned long long expected_length = folder_content.length();
    unsigned long long sended = 0;

    while (sended < expected_length) {
        int send_length = size;                         //velkost celeho bufferu
        if (send_length + sended > expected_length)     //ak by to precilo velkost velkost celeho stringu
            send_length = expected_length - sended + 1; //tak tam zapis iba zvysok a plus nulty byte

        switch (sended += send(client_socket, folder_content.c_str() + sended, send_length, 0)) {
            case -1: break;
            case  0: break;
            default: break;
        }      
    }

    return 1;//read_size;
}

int mkdResponse (int client_socket, char* buffer, int size, HTTP_message_t message, string root_path) {
    struct stat statbuf;

    cout<< root_path + message["Remote-Path"] << endl;

    mkdir((root_path + message["Remote-Path"]).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    message["Code"] = "200 OK";
    
    message["Content-Encoding"] = "identity"; 
    message["Content-Type"] = "text/plain";
    message["Content-Length"] = "0";
    message.writeResponseHead(buffer, size);
    switch (send(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }      
    
    return 1;//read_size;
}

int rmdResponse (int client_socket, char* buffer, int size, HTTP_message_t message, string root_path) {
    struct stat statbuf;

    if (stat((root_path + message["Remote-Path"]).c_str(), &statbuf) != 0)
        message["Code"] = "404 Not Found";   
    
    else if (!S_ISDIR(statbuf.st_mode))
        message["Code"] = "400 Bad Request";
    
    else { 
        rmdir((root_path + message["Remote-Path"]).c_str());
        message["Code"] = "200 OK";
    }

    message["Content-Encoding"] = "identity"; 
    message["Content-Type"] = "text/plain";
    message["Content-Length"] = "0";
    message.writeResponseHead(buffer, size);
    switch (send(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }      
    
    return 1;//read_size;
}

int delResponse(int client_socket, char* buffer, int size, HTTP_message_t message, string root_path) {
    struct stat statbuf;

    if (stat((root_path + message["Remote-Path"]).c_str(), &statbuf) != 0)
        message["Code"] = "404 Not Found";   
    
    else if (!S_ISREG(statbuf.st_mode))
        message["Code"] = "400 Bad Request";
    
    else { 
        remove((root_path + message["Remote-Path"]).c_str());
        message["Code"] = "200 OK";
    }

    message["Content-Encoding"] = "identity"; 
    message["Content-Type"] = "text/plain";
    message["Content-Length"] = "0";
    message.writeResponseHead(buffer, size);
    switch (send(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }      
    
    return 1;//read_size;
}
