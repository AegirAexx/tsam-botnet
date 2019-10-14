// COM: T-409-TSAM-2019-3 | Project 3 - The Botnet Rises
// COM: Aegir_Tomasson[aegir15] && Dagur_Kristjansson[dagur17]


// COM: This is our datatype to hold messages and log to file.
// COM:
// COM: Message API:
// COM:  public:
// COM:      Utilities()
// COM:
// COM:      size_t getTimestamp() - Class getter.
// COM:      string getLocalIP - Class getter.
// COM:
// COM:      vector<string> split(string stringToSplit, char delimeter) - Private utility function.
// COM:      bool validateCommand(Command cmd) - Valdation API.
// COM:
// COM:  private:
// COM:      bool isCONNECT(std::vector<std::string> payload); - Provate validator.
// COM:      bool isGETMSG(std::vector<std::string> payload); - Provate validator.
// COM:      bool isSENDMSG(std::vector<std::string> payload); - Provate validator.
// COM:      bool isSEND_MSG(std::vector<std::string> payload); - Provate validator.
// COM:      bool isKEEPALIVE(std::vector<std::string> payload); - Provate validator.
// COM:      bool isLEAVE(std::vector<std::string> payload); - Provate validator.
// COM:      bool isSERVERS(std::vector<std::string> payload); - Provate validator.
// COM:      bool isSTATUSRESP(std::vector<std::string> payload); - Provate validator.
// COM:      bool isSTATUSREQ(std::vector<std::string> payload); - Provate validator.
// COM:      bool isLISTSERVERS(std::vector<std::string> payload); - Provate validator.
// COM:

#include "Utilities.h"

// COM: Class constructor.
Utilities::Utilities() {}

// COM: Retruns an unsigned int with the number of seconds since Epoch.
std::size_t Utilities::getTimestamp() {
    // COM: A call to chrono duration with the current time and Epoch.
    return (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

// COM: Returns host IPv4 number. Reverse engineered from Jacky's hand out code.
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

// COM: Utility function that returns a string where 0x01 and 0x04 have been added to the front and back.
std::string Utilities::addRawBytes(std::string str){
    // COM: A container for the raw bytes.
    std::vector<std::byte> rawBytes;
    // COM: Placeholder for the return string.
    std::string reformattedString;
    // COM: Pushing 0x01 to the front.
    rawBytes.push_back((std::byte)0x01);
    // COM: Followed by the rest of the string.
    for(auto charByte: str) rawBytes.push_back((std::byte)charByte);
    // COM: Finally 0x04 is pushed to the back.
    rawBytes.push_back((std::byte)0x04);
    // COM: Combine everything in the container into a new string to be returned.
    for(auto byte: rawBytes) reformattedString.push_back((char)byte);
    // COM: Return reformatted string.
    return reformattedString;

}

// COM: Utility function that returns a string where 0x01 and 0x04 have been removed from the front and back.
std::string Utilities::removeRawBytes(std::string str){
    // COM: A container for the raw bytes.
    std::vector<std::byte> rawBytes;
    // COM: Placeholder for the return string.
    std::string reformattedString;
    // COM: Flags for the 0x01 and 0x04 bytes.
    bool isSoh{false};
    bool isEot{false};
    // COM: Pushing the string onto the container. Keeping an eye out for those hexes.
    for(auto charByte: str) {
        if(charByte == 0x01) isSoh = true;
        if(charByte == 0x04) isEot = true;
        rawBytes.push_back((std::byte)charByte);
    }
    // COM: If those hexes were seen they are removed.
    if(isEot) rawBytes.pop_back();
    if(isSoh) rawBytes.erase(rawBytes.begin());
    // COM: Combine everything in the container into a new string to be returned.
    for(auto byte: rawBytes) reformattedString.push_back((char)byte);
    // COM: Return reformatted string.
    return reformattedString;
}

// TODO: DELETE???
std::string Utilities::handshake(std::string groupName) {
    return addRawBytes("LISTSERVERS," + groupName);
}


// COM: Utility function that splits a string according to a delimiter and returns a vector of single words or tokens.
std::vector<std::string> Command::split(std::string stringToSplit, char delimeter) {
    // COM: Creating a string stream object from input string.
    std::stringstream ss(stringToSplit);
    // COM: Temporary placeholder.
    std::string word;
    // COM: Container to hold words or tokens.
	std::vector<std::string> splittedStrings;
    // COM: While getline is fed strings from the string stream object, they are push onto the container.
    while (std::getline(ss, word, delimeter)) splittedStrings.push_back(word);
    // COM: The container is returned.
    return splittedStrings;
}

// COM: Function that validates the command string. A lot of our logic is based on the server-2-server comunication.
// COM: It is therefore very important to not let through the commands that might crash our application.
bool Utilities::validateCommand(Command cmd) {
    // COM: Decision matrix. Calls to private functions.
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

// COM: The following functions are all part of validateCommand(). There is a function for each known command.

// COM: They are all very similar in the way that they use regular expresion.
// COM: Regular expresions:
// COM: "(^P3_GROUP_\\d+|^I[a-z]+_\\d+)" - Validates the group names.
// COM: "\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}" - Validates the IPv4 number.
// COM: "^[1-5]?\\d{4}$" - Validates the port number.
// COM: "^[\\d]$" - Validates if it's a number.


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
