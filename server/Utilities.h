// COM: T-409-TSAM-2019-3 | Project 3 - The Botnet Rises
// COM: Aegir_Tomasson[aegir15] && Dagur_Kristjansson[dagur17]

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

// COM: This is our collection of utility fuctions.

class Utilities {
    public:
    // COM: Class constructor.
        Utilities();

    // COM: Class utility getters.
        size_t getTimestamp();
        std::string getLocalIP();

    // COM: Split utility function.
        std::vector<std::string> split(std::string stringToSplit, char delimeter);
        bool validateCommand(Command cmd);

    // COM: Functions for manipulating SOH and EOT.
        std::string addRawBytes(std::string str);
        std::string removeRawBytes(std::string str);

    // COM: Our handshake protocol.
        std::string handshake(std::string groupName);


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

