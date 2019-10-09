#ifndef UTILITIES_H
#define UTILITIES_H

#include "Command.h"
//#include "Client.h"

#include <iostream>
#include <regex>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <iterator>

// TODO: Are all of these includes needed?
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Utilities {
    public:
        Utilities();

        std::size_t getTimestamp();
        std::string getLocalIP();

        int idCommand(const char *buffer);

        std::vector<std::string> split(std::string stringToSplit, char delimeter);
        void listCommands();
        std::vector<Command> processPayload(const std::string payload);

        // SECTION: NETWORKING
        //void closeClient(std::vector<Client> clients, int clientSocket, fd_set *openSockets, int *maxfds);
        //void clientCommand(std::vector<Client> clients, int clientSocket, fd_set *openSockets, int *maxfds, char *buffer);

        //Adding start stop
        std::vector<std::byte> getRawBytes(std::string str);
        std::string rebuildString(std::string str);



    private:

};

#endif // UTILITIES_H

