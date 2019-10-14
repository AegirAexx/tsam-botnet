#ifndef UTILITIES_H
#define UTILITIES_H

#include "Command.h"

#include <ctime>
#include <chrono>
#include <vector>
#include <iterator>
#include <sstream>
#include <regex>
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
        bool validateCommand(Command cmd);

        // start stop | SOH - EOT
        std::string addRawBytes(std::string str);
        std::string removeRawBytes(std::string str);

        // Handshake
        std::string handshake(std::string groupName, std::string ipAddress, int port);


    private:
        bool isCONNECT(std::vector<std::string> payload);
        bool isGETMSG(std::vector<std::string> payload);
        bool isSENDMSG(std::vector<std::string> payload);
        bool isSEND_MSG(std::vector<std::string> payload);
        bool isKEEPALIVE(std::vector<std::string> payload);
        bool isLEAVE(std::vector<std::string> payload);
        bool isSERVERS(std::vector<std::string> payload);
        bool isSTATUSRESP(std::vector<std::string> payload);
        bool isSTATUSREQ(std::vector<std::string> payload);
        bool isLISTSERVERS(std::vector<std::string> payload);

};


#endif // UTILITIES_H

