
#include "../../include/data/ServerProfileData.h"

ServerProfileData::ServerProfileData(){}

std::vector<ServerProfile> ServerProfileData::getServers(){
    std::vector<ServerProfile> servers;
    std::ifstream input;
    input.open("data/ServerProfileData", std::ios::binary);
    input.seekg(0, input.end);
    size_t totalLogged {input.tellg() / sizeof(ServerProfile)};
    input.seekg(0, input.beg);
    ServerProfile *dataPointer = new ServerProfile[totalLogged];
    input.read((char*)dataPointer, sizeof(ServerProfile) * totalLogged);
    input.close();
    for(size_t i{0}; i < totalLogged; ++i){
        servers.push_back(dataPointer[i]);
    }
    // TODO: MEMORY LEAKS??
    // delete [] dataPointer; // BUG:
    return servers;
}

void ServerProfileData::write(ServerProfile server){

    std::ofstream output;
    output.open("data/ServerProfileData", std::ios::app | std::ios::binary);
    output.write((char*)(&server), sizeof(ServerProfile));
    output.close();

}

bool ServerProfileData::checkFile(bool x){
    return !x;
}