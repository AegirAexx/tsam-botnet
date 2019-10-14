
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
    return addRawBytes("LISTSERVERS," + groupName);
}


std::vector<std::string> Utilities::split(std::string stringToSplit, char delimeter) {
    std::stringstream ss(stringToSplit);
    std::string word;
	std::vector<std::string> splittedStrings;
    while (std::getline(ss, word, delimeter)) splittedStrings.push_back(word);
    return splittedStrings;
}

// PART: VALIDATION SERVICE

bool Utilities::validateCommand(Command cmd) {
    if(cmd.getID() == 1) return isSEND_MSG(cmd.getPayload());
    if(cmd.getID() == 2) return isGETMSG(cmd.getPayload());
    if(cmd.getID() == 3) return isKEEPALIVE(cmd.getPayload());
    if(cmd.getID() == 4) return isCONNECT(cmd.getPayload());
    if(cmd.getID() == 5) return isLEAVE(cmd.getPayload());
    if(cmd.getID() == 6) return isSTATUSREQ(cmd.getPayload());
    if(cmd.getID() == 7) return isLISTSERVERS(cmd.getPayload());
    if(cmd.getID() == 8) return isSERVERS(cmd.getPayload());
    if(cmd.getID() == 9) return isSTATUSRESP(cmd.getPayload());
    if(cmd.getID() == 10) return isGETMSG(cmd.getPayload());
    if(cmd.getID() == 11) return isSENDMSG(cmd.getPayload());
    else if (cmd.getID() != 1) return false;
    else return false;
}


bool Utilities::isCONNECT(std::vector<std::string> payload) {
    if(!(payload.size() == 3)) return false;
    std::regex rxGroup("(^P3_GROUP_\\d+|^I[a-z]+_\\d+)");
    std::smatch matchGroup;
    std::regex_search(payload[0], matchGroup, rxGroup);
    bool isGroup{!matchGroup.empty()};
    std::regex rxIpAddr("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");
    std::smatch matchIpAddr;
    std::regex_search(payload[1], matchIpAddr, rxIpAddr);
    bool isIpAddr{!matchIpAddr.empty()};
    std::regex rxPort("^[1-5]?\\d{4}$");
    std::smatch matchPort;
    std::regex_search(payload[2], matchPort, rxPort);
    bool isPort{!matchPort.empty()};
    return (isGroup && isIpAddr && isPort);
}


bool Utilities::isGETMSG(std::vector<std::string> payload) {
    if(!(payload.size() == 1)) return false;
    std::regex rxGroup("(^P3_GROUP_\\d+|^I[a-z]+_\\d+)");
    std::smatch matchGroup;
    std::regex_search(payload[0], matchGroup, rxGroup);
    return !matchGroup.empty();
}


bool Utilities::isSENDMSG(std::vector<std::string> payload) {
    if(!(payload.size() == 2)) return false;
    std::regex rxGroup("(^P3_GROUP_\\d+|^I[a-z]+_\\d+)");
    std::smatch matchGroup;
    std::regex_search(payload[0], matchGroup, rxGroup);
    return !matchGroup.empty();
}


bool Utilities::isSEND_MSG(std::vector<std::string> payload) {
    if(!(payload.size() == 3)) return false;
    std::regex rxGroup("(^P3_GROUP_\\d+|^I[a-z]+_\\d+)");
    std::smatch matchGroupFrom;
    std::regex_search(payload[0], matchGroupFrom, rxGroup);
    std::smatch matchGroupTo;
    std::regex_search(payload[1], matchGroupTo, rxGroup);
    return !matchGroupFrom.empty() && !matchGroupTo.empty();
}


bool Utilities::isKEEPALIVE(std::vector<std::string> payload) {
    if(!(payload.size() == 1)) return false;
    std::regex rxNumber("^[\\d]$");
    std::smatch matchNumber;
    std::regex_search(payload[0], matchNumber, rxNumber);
    return !matchNumber.empty();
}


bool Utilities::isLEAVE(std::vector<std::string> payload) {
    if(!(payload.size() == 2)) return false;
    std::regex rxIpAddr("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");
    std::smatch matchIpAddr;
    std::regex_search(payload[0], matchIpAddr, rxIpAddr);
    bool isIpAddr{!matchIpAddr.empty()};
    std::regex rxPort("^[1-5]?\\d{4}$");
    std::smatch matchPort;
    std::regex_search(payload[1], matchPort, rxPort);
    bool isPort{!matchPort.empty()};
    return isPort && isIpAddr;
}


bool Utilities::isSERVERS(std::vector<std::string> payload) {
    if(!(payload.size() >= 3)) return false;
    std::regex rxIpAddr("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");
    std::smatch matchIpAddr;
    std::regex_search(payload[1], matchIpAddr, rxIpAddr);
    bool isIpAddr{!matchIpAddr.empty()};
    std::regex rxPort("^[1-5]?\\d{4}$");
    std::smatch matchPort;
    std::regex_search(payload[2], matchPort, rxPort);
    bool isPort{!matchPort.empty()};
    return isPort && isIpAddr;
}


bool Utilities::isSTATUSRESP(std::vector<std::string> payload) {
    if(!(payload.size() >= 4)) return false;
    std::regex rxGroup("(^P3_GROUP_\\d+|^I[a-z]+_\\d+)");
    std::smatch matchGroupFrom;
    std::regex_search(payload[0], matchGroupFrom, rxGroup);
    std::smatch matchGroupTo;
    std::regex_search(payload[1], matchGroupTo, rxGroup);
    std::regex rxNumber("^[\\d]$");
    std::smatch matchNumber;
    std::regex_search(payload[3], matchNumber, rxNumber);
    return !matchGroupFrom.empty() && !matchGroupTo.empty() && !matchNumber.empty();
}


bool Utilities::isSTATUSREQ(std::vector<std::string> payload) {
    if(!(payload.size() == 1)) return false;
    std::regex rxGroup("(^P3_GROUP_\\d+|^I[a-z]+_\\d+)");
    std::smatch matchGroup;
    std::regex_search(payload[0], matchGroup, rxGroup);
    return !matchGroup.empty();
}


bool Utilities::isLISTSERVERS(std::vector<std::string> payload) {
    if(!(payload.size() == 1)) return false;
    std::regex rxGroup("(^P3_GROUP_\\d+|^I[a-z]+_\\d+)");
    std::smatch matchGroup;
    std::regex_search(payload[0], matchGroup, rxGroup);
    return !matchGroup.empty();
}

