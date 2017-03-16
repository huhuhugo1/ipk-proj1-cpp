#include <regex.h>      

using namespace std;

bool regex_match(string input, string pattern) {
    regex_t regex;

    bool result;

    if (regcomp(&regex, pattern.c_str(), REG_EXTENDED)) {
        result = false;
        cout << "error";
    }
    else
        switch (regexec(&regex, input.c_str(), 0, NULL, 0)) {
            case 0:
                result = true;
                break;
            default:
                result = false;
                break;
        }

    regfree(&regex);

    return result;
}
