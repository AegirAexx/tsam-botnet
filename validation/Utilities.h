#ifndef UTILITIES_H
#define UTILITIES_H

#include <ctime>
#include <chrono>
#include <vector>
#include <iterator>
#include <sstream>
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
        std::vector<std::string> split(std::string stringToSplit, char delimeter);


        // start stop | SOH - EOT
        std::string addRawBytes(std::string str);
        std::string removeRawBytes(std::string str);

        // Handshake
        std::string handshake(std::string groupName, std::string ipAddress, int port);


    private:

};

#endif // UTILITIES_H