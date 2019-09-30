#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <regex>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <iterator>
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
        int idCommand(std::string payload);
        std::vector<std::string> split(std::string stringToSplit, char delimeter);

    private:

};

#endif // UTILITIES_H

