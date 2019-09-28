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

std::ostream& operator << (std::ostream& outs, const ServerProfile& profile){
    ServerProfile tempProfile = profile; // Temporary variable to circumvent "const" restrictions.
    outs << "Server " << tempProfile.group
         << "; Port " << tempProfile.port
         << "; IPv4 " << tempProfile.ip
    << std::endl;
    return outs;
}


// outs << std::setfill ('0') << std::setw (2) << tempProfile.hours << ":"
//      << std::setfill ('0') << std::setw (2) << tempProfile.minutes << ":"
//      << std::setfill ('0') << std::setw (2) << tempProfile.seconds << std::endl;
