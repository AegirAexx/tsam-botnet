
#include "Utilities.h"

Utilities::Utilities() {}

// PART: Functions that are working with other code. Ready??

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

std::string Utilities::addRawBytes(std::string str){
    std::vector<std::byte> rawBytes;
    std::string reformattedString;
    rawBytes.push_back((std::byte)0x01);
    for(auto charByte: str) rawBytes.push_back((std::byte)charByte);
    rawBytes.push_back((std::byte)0x04);
    for(auto byte: rawBytes) reformattedString.push_back((char)byte);
    return reformattedString;

}

std::string Utilities::removeRawBytes(std::string str){
    std::vector<std::byte> rawBytes;
    std::string reformattedString;
    bool isSoh{false};
    bool isEot{false};
    for(auto charByte: str) {
        if(charByte == 0x01) isSoh = true;
        if(charByte == 0x04) isEot = true;
        rawBytes.push_back((std::byte)charByte);
    }
    if(isEot) rawBytes.pop_back();
    if(isSoh) rawBytes.erase(rawBytes.begin());
    for(auto byte: rawBytes) reformattedString.push_back((char)byte);
    return reformattedString;
}

std::string Utilities::handshake(std::string groupName, std::string ipAddress, int port) {
    return addRawBytes("LISTSERVERS," + groupName + "," + ipAddress + "," + std::to_string(port));
}

std::vector<std::string> Utilities::split(std::string stringToSplit, char delimeter) {
    std::stringstream ss(stringToSplit);
    std::string word;
	std::vector<std::string> splittedStrings;
    while (std::getline(ss, word, delimeter)) splittedStrings.push_back(word);
    return splittedStrings;
}
