#include <dirent.h>

int putResponse(int client_socket, char* buffer, int size, HTTP_message_t message) {
    FILE* file = fopen(("." + message["Remote-Path"]).c_str(), "wb");
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

int lstResponse(int client_socket, char* buffer, int size, HTTP_message_t message) {
    string folder_content;

    if (DIR *dpdf = opendir(("." + message["Remote-Path"]).c_str()))
        while (struct dirent *epdf = readdir(dpdf))
            folder_content.append(epdf->d_name).append("\n");
    else 
        ;//todo error
    
    //todo zapsani hlavičky do stringu/bufferu

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

/*
int lstResponse(int client_socket, char* buffer, int size, HTTP_message_t message) {
    string file_list;
    int read_size;

    
    
    int start = 0;
    int n = 0;
    int length = file_list

        do {
            if (start + size <= length)
                n = size;
            else
                n = length - start;
            send(client_socket, file_list.substr(start, n).c_str(), size, 0);
            start += size;
        } while (start < length);;
    
    return 0;
}
*/
