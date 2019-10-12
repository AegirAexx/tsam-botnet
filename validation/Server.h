#ifndef SERVER_H
#define SERVER_H

#include <string>

class Server {
    public:
        Server(int socket);
        ~Server();

        int getSocket();
        int getPort();
        std::string getName();
        std::string getIpAddress();
        bool getIsCOC();

    private:
        int sock;
        int port;
        std::string name;
        std::string ipAddress;
        bool isCOC;

};


#endif // SERVER_H