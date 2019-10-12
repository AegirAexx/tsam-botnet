#ifndef SERVERPROFILEDATA_H
#define SERVERPROFILEDATA_H

#include <fstream>
#include <vector>
#include "../model/ServerProfile.h"

#include <iostream> // DEBUG:


class ServerProfileData{
    public:
        ServerProfileData();
        virtual ~ServerProfileData();

        std::vector<ServerProfile> getServers();
        void write(ServerProfile server);
        bool checkFile(bool x);
    private:
        ServerProfile *dataPointer;

};

#endif // SERVERPROFILEDATA_H
