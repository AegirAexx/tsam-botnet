#ifndef SERVER_H
#define SERVER_H

#include <string>

class Server {
    public:
        Server(std::string name) : name("William") {}
        virtual ~Server() {}
        // Some kind of public variable
        std::string name;
};

#endif // SERVER_H