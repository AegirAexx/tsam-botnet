#ifndef SERVERPROFILE_H
#define SERVERPROFILE_H

#include <cstring>
#include <ostream>

class ServerProfile {
    public:
        ServerProfile();
        ServerProfile(int port, std::string ip, std::string group);
        virtual ~ServerProfile();

        friend std::ostream& operator << (std::ostream& outs, const ServerProfile& serverProfile);

    private:
        int port;
        char ip[16];
        char group[12];
};

#endif // SERVERPROFILE_H

