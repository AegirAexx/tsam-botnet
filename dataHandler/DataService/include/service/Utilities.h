#ifndef UTILITIES_H
#define UTILITIES_H

// #include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>
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

    private:

};

#endif // UTILITIES_H

