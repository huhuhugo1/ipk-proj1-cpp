#include <regex>

using namespace std;

struct argumentBox {
    string command;
    string host;
    unsigned port;
    string remote_path;
    string local_path;

    argumentBox(int argc, const char * argv[]) {
        this->port = 0;
        if (argc <= 2) {
            perror("Invalid number of parameters.");
            exit(1);
        }
        
        if (argc >= 3) {
            this->command.assign(argv[1]);

            string url(argv[2]);
            if (regex_match(url, regex("^http:\\/\\/[^:]+:[0-9]+\\/.*$"))) {
                url.assign(argv[2]+7);    

                int semicolon_idx = url.find(':', 0);
                int path_start_idx = url.find('/', semicolon_idx);
                
                this->host.assign(url.substr(0, semicolon_idx));
                this->port = atoi(url.substr(semicolon_idx + 1, path_start_idx - semicolon_idx - 1).c_str());
                this->remote_path.assign(url.substr(path_start_idx, -1));
            } else {
                perror("Invalid format of url.");
                exit(1);    
            }

            if (argc >= 4) {
                this->local_path.assign(argv[3]);
            }
        }

    }
};
