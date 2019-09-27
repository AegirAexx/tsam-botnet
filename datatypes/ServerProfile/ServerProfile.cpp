// TODO: VALIDATION!!

#include "ServerProfile.h"

ServerProfile::ServerProfile(){
    this->port = 0;
    strcpy (this->ip, "127.0.0.1");
    strcpy (this->group, "NOID_NOID");
}

ServerProfile::ServerProfile(int port, std::string ip, std::string group){
    this->port = port;
    strcpy (this->ip, ip.c_str());
    strcpy (this->group, group.c_str());
}

ServerProfile::~ServerProfile() {}