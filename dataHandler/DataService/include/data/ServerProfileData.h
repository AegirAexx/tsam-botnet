#ifndef SERVERPROFILEDATA_H
#define SERVERPROFILEDATA_H

#include <fstream>
#include <vector>
#include "../model/ServerProfile.h"


class ServerProfileData{
    public:
        ServerProfileData();
        std::vector<ServerProfile> getServers();
        // Set as void, but maybe should be an int? For error checking?
        void write(ServerProfile server);
        // Some kind of validation?
        bool checkFile(bool x);

    // private:
    // No private members or variables.

};

#endif // SERVERPROFILEDATA_H
