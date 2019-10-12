#ifndef CLIENT_H
#define CLIENT_H

#include <cstring>
#include <ostream>


class Client {
    public:
        Client(int socket, std::string name);
        ~Client();
        friend std::ostream& operator << (std::ostream& outs, const Client& client);

    private:
        int sock;
        char name[13]; // FEAT: Does this have to be static?
};

#endif // CLIENT_H
