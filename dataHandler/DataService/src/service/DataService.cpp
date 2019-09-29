
#include "../../include/service/DataService.h"

DataService::DataService(){}

// DEBUG:
// std::vector<ServerProfile> DataService::getServers(){
//     std::vector<ServerProfile> servers;
//     ServerProfile server1(4567, "234.123.123.123", "V_GROUP_123");
//     ServerProfile server2(4557, "123.234.123.123", "V_GROUP_143");
//     ServerProfile server3(4547, "123.123.234.123", "V_GROUP_163");
//     ServerProfile server4(4537, "123.123.123.234", "V_GROUP_183");
//     servers.push_back(server1);
//     servers.push_back(server2);
//     servers.push_back(server3);
//     servers.push_back(server4);
//     return servers;
// }

std::vector<ServerProfile> DataService::getServers(){

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

    delete [] dataPointer;

    return servers;
}

void DataService::write(ServerProfile server){

    std::ofstream output;
    output.open("data/ServerProfileData", std::ios::binary|std::ios::app);
    output.write((char*)(&server), sizeof(ServerProfile));
    output.close();

}

bool DataService::checkFile(bool x){
    return !x;
}

// PART: THROWAWAY CODE DEBUG:
// std::vector<std::string> logs;
// std::string lineLog;
// std::ifstream messageLogs;
// messageLogs.open("logs.txt");
// while (std::getline(messageLogs, lineLog)){
//     logs.push_back(lineLog);
// }
// messageLogs.close();