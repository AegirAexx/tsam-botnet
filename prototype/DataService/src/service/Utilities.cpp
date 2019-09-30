
#include "../../include/service/Utilities.h"

Utilities::Utilities() {}

std::size_t Utilities::getTimestamp() {
    return (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string Utilities::getLocalIP() {
    struct ifaddrs *myAddress, *ifa;
    void *in_addr;
    char tempBuffer[64];
    getifaddrs(&myAddress);
    for (ifa = myAddress; ifa != NULL; ifa = ifa->ifa_next){
        if(ifa->ifa_addr->sa_family == AF_INET && (std::string)ifa->ifa_name != "lo"){
            struct sockaddr_in *s4 {(struct sockaddr_in *)ifa->ifa_addr};
            in_addr = &s4->sin_addr;
            inet_ntop(ifa->ifa_addr->sa_family, in_addr, tempBuffer, sizeof(tempBuffer));
            return tempBuffer;
        }
    }
    return "error";
}

std::vector<std::string> split(std::string stringToSplit, char delimeter) {

    if(std::any_of(std::begin(stringToSplit), std::end(stringToSplit), [=](char c) { return c == delimeter; })) {
        std::cout << "Yes it has a: " << delimeter << std::endl;
    }


    std::stringstream ss(stringToSplit);
    std::string item;
	std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter)) splittedStrings.push_back(item);
	return splittedStrings;
}
