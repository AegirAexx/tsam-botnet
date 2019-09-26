#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client {
    public:
        Client(int sock, std::string name) : sock(0), name("John") {}
        virtual ~Client() {}

        int sock; // socket of client connection
        std::string name; // Limit length of name of client's user
};

#endif // CLIENT_H
