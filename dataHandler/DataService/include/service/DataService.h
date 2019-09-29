#ifndef DATASERVICE_H
#define DATASERVICE_H

#include <fstream>
#include <vector>
#include "../model/ServerProfile.h"


class DataService{
    public:
        DataService();
        std::vector<ServerProfile> getServers();
        // Set as void, but maybe should be an int? For error checking?
        void write(ServerProfile server);
        // Some kind of validation?
        bool checkFile(bool x);

    // private:
    // No private members or variables.

};

#endif // DATASERVICE_H
