#include <dirent.h>
#include <sys/stat.h>

void sendError (char* buffer, int size, int client_socket, string message) {
    char* body_pos = strstr(buffer, "\r\n\r\n") + 4;
    strcpy(body_pos, message.c_str());

    switch (send(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }
}

void mkdResponse (int client_socket, char* buffer, int size, HTTP_message_t message, string root_path) {
    string error_msg = "";
    struct stat statbuf;

    if (mkdir((root_path + message["Remote-Path"]).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
        message["Code"] = "200 OK";
        message["Content-Encoding"] = "identity"; 
        message["Content-Type"] = "text/plain";
        message["Content-Length"] = "0";
    } 
    else if (errno == EEXIST) {
        error_msg = "Already exists.\n";
        message["Code"] = "400 Bad Request";
        message["Content-Encoding"] = "identity"; 
        message["Content-Type"] = "text/plain";
        message["Content-Length"] = to_string(error_msg.length());
    } 
    else {
        error_msg = "Unknown error.\n";
        message["Code"] = "400 Bad Request";
        message["Content-Encoding"] = "identity"; 
        message["Content-Type"] = "text/plain";
        message["Content-Length"] = to_string(error_msg.length());
    }

    message.writeResponseHead(buffer, size);

    if (error_msg != "") {
        sendError(buffer, size, client_socket, error_msg);
        return;
    }

    switch (send(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }      
}

void rmdResponse (int client_socket, char* buffer, int size, HTTP_message_t message, string root_path) {
    string error_msg = "";
    struct stat statbuf;

    if (stat((root_path + message["Remote-Path"]).c_str(), &statbuf) != 0) {
        error_msg = "Directory not found.\n";
        message["Code"] = "404 Not Found";
        message["Content-Encoding"] = "identity"; 
        message["Content-Type"] = "text/plain";
        message["Content-Length"] = to_string(error_msg.length());   
    }
    else if (!S_ISDIR(statbuf.st_mode)) {
        error_msg = "Not a directory.\n";
        message["Code"] = "400 Bad Request";
        message["Content-Encoding"] = "identity"; 
        message["Content-Type"] = "text/plain";
        message["Content-Length"] = to_string(error_msg.length());
    }
    else { 
        if (rmdir((root_path + message["Remote-Path"]).c_str()) == 0) {
            message["Code"] = "200 OK";
            message["Content-Encoding"] = "identity"; 
            message["Content-Type"] = "text/plain";
            message["Content-Length"] = "0";
        }
        else {
            if (errno == ENOTEMPTY)
                error_msg = "Directory not empty.\n";
            else
                error_msg = "Unknown error.\n";

            message["Code"] = "400 Bad Request";
            message["Content-Encoding"] = "identity"; 
            message["Content-Type"] = "text/plain";
            message["Content-Length"] = to_string(error_msg.length());
        }
    }
    
    message.writeResponseHead(buffer, size);
    
    if (error_msg != "") {
        sendError(buffer, size, client_socket, error_msg);
        return;
    }
    
    switch (send(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }      
}

void lstResponse (int client_socket, char* buffer, int size, HTTP_message_t message, string root_path) {
    string error_msg = "";
    string folder_content;
    struct stat statbuf;

    if (stat((root_path + message["Remote-Path"]).c_str(), &statbuf) != 0) {
        error_msg = "Directory not found.\n";
        message["Code"] = "404 Not Found"; 
        message["Content-Encoding"] = "identity"; 
        message["Content-Type"] = "text/plain";  
        message["Content-Length"] = to_string(error_msg.length());
    }
    else if (!S_ISDIR(statbuf.st_mode)) {
        error_msg = "Not a directory.\n";
        message["Code"] = "400 Bad Request";
        message["Content-Encoding"] = "identity"; 
        message["Content-Type"] = "text/plain";
        message["Content-Length"] = to_string(error_msg.length());
    }
    else {
        if (DIR *dpdf = opendir((root_path + message["Remote-Path"]).c_str())) {
            while (struct dirent *epdf = readdir(dpdf))
                folder_content.append(epdf->d_name).append("\n");
            
            message["Code"] = "200 OK";
            message["Content-Encoding"] = "identity"; 
            message["Content-Type"] = "text/plain";
            message["Content-Length"] = to_string(folder_content.length());
        } 
        else {
            error_msg = "Unknown error.\n";
            message["Code"] = "400 Bad Request";
            message["Content-Encoding"] = "identity"; 
            message["Content-Type"] = "text/plain";
            message["Content-Length"] = to_string(error_msg.length());
        }
    }
    
    message.writeResponseHead(buffer, size);

    if (error_msg != "") {
        sendError(buffer, size, client_socket, error_msg);
        return;
    }

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
}

void delResponse (int client_socket, char* buffer, int size, HTTP_message_t message, string root_path) {
    string error_msg = "";
    struct stat statbuf;

    if (stat((root_path + message["Remote-Path"]).c_str(), &statbuf) != 0) {
        error_msg = "File not found.\n";
        message["Code"] = "404 Not Found"; 
        message["Content-Encoding"] = "identity"; 
        message["Content-Type"] = "text/plain";  
        message["Content-Length"] = to_string(error_msg.length());   
    }
    else if (!S_ISREG(statbuf.st_mode)) {
        error_msg = "Not a file.\n";
        message["Code"] = "400 Bad Request";
        message["Content-Encoding"] = "identity"; 
        message["Content-Type"] = "text/plain";
        message["Content-Length"] = to_string(error_msg.length());
    }
    else { 
        if (remove((root_path + message["Remote-Path"]).c_str()) == 0) {
            message["Code"] = "200 OK";
            message["Content-Encoding"] = "identity"; 
            message["Content-Type"] = "text/plain";
            message["Content-Length"] = "0";
        }
        else {
            error_msg = "Unknown error.\n";
            message["Code"] = "400 Bad Request";
            message["Content-Encoding"] = "identity"; 
            message["Content-Type"] = "text/plain";
            message["Content-Length"] = to_string(error_msg.length());
        }
    }

    message.writeResponseHead(buffer, size);

    if (error_msg != "") {
        sendError(buffer, size, client_socket, error_msg);
        return;
    }
    
    switch (send(client_socket, buffer, size, 0)) {
        case -1: break;
        case  0: break;
        default: break;
    }      
}

void getResponse (int client_socket, char* buffer, int size, HTTP_message_t message, string root_path) {
    string error_msg = "";
    unsigned long long expected_length;
    unsigned long long read = 0;
    FILE* file = NULL;
    
    struct stat statbuf;
    if (stat((root_path + message["Remote-Path"]).c_str(), &statbuf) != 0) {
        message["Code"] = "404 Not Found";
        message["Content-Encoding"] = "identity"; 
        message["Content-Type"] = "text/plain";
        message["Content-Length"] = "0";
        expected_length = 0;
    }
    else if (!S_ISREG(statbuf.st_mode)) {
        message["Code"] = "400 Bad Request";
        message["Content-Encoding"] = "identity"; 
        message["Content-Type"] = "text/plain";
        message["Content-Length"] = "0";
        expected_length = 0;
    }
    else {  
        if (file = fopen((root_path + message["Remote-Path"]).c_str(), "rb")) {
            fseek(file, 0L, SEEK_END);
            expected_length = ftell(file);
            rewind(file);
        
            message["Code"] = "200 OK";
            message["Content-Encoding"] = "identity"; 
            message["Content-Type"] = "application/octet-stream";
            message["Content-Length"] = to_string(expected_length);
        }
        else {
            error_msg = "Unknown error.\n";
            message["Code"] = "400 Bad Request";
            message["Content-Encoding"] = "identity"; 
            message["Content-Type"] = "text/plain";
            message["Content-Length"] = to_string(error_msg.length());
        }
    }
    
    message.writeResponseHead(buffer, size);

    if (error_msg != "") {
        sendError(buffer, size, client_socket, error_msg);
        return;
    }

    char* body_pos = strstr(buffer, "\r\n\r\n") + 4;

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

void putResponse (int client_socket, char* buffer, int size, HTTP_message_t message, string root_path) {
    string error_msg = "";
    FILE* file;

    if (access( (root_path + message["Remote-Path"]).c_str(), F_OK ) != -1) {
        error_msg = "Already exists.\n";
        message["Code"] = "400 Bad Request";
        message["Content-Encoding"] = "identity"; 
        message["Content-Type"] = "text/plain";
        message["Content-Length"] = to_string(error_msg.length());
    } 
    else if ((file = fopen((root_path + message["Remote-Path"]).c_str(), "wb")) == NULL) {
        error_msg = "Unknown error.\n";
        message["Code"] = "400 Bad Request";
        message["Content-Encoding"] = "identity"; 
        message["Content-Type"] = "text/plain";
        message["Content-Length"] = to_string(error_msg.length());
    }

    if (error_msg != "") {
        message.writeResponseHead(buffer, size);
        sendError(buffer, size, client_socket, error_msg);
        return;
    }
    
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
}
