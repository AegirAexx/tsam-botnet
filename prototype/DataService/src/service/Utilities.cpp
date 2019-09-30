
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

// DEBUG:
 void Utilities::listCommands() {
    std::cout << "############ KNOWN COMMANDS ############" << std::endl;
    std::cout << "~ NO COMMAND - #(-1)" << std::endl;
    std::cout << "~ UNKNOWN COMMAND - MADE IT THROUGH THE FILTER - COMMAND #(0)" << std::endl;
    std::cout << "SEND MSG - COMMAND #(1)" << std::endl;
    std::cout << "GET MSG - COMMAND #(2)" << std::endl;
    std::cout << "KEEPALIVE - COMMAND #(3)" << std::endl;
    std::cout << "CONNECT - COMMAND #(4)" << std::endl;
    std::cout << "LEAVE - COMMAND #(5)" << std::endl;
    std::cout << "STATUSREQ - COMMAND #(6)" << std::endl;
    std::cout << "LISTSERVERS - COMMAND #(7)" << std::endl;
    std::cout << "########################################" << std::endl;
}

// (?<=,)\s?[\w+\.]+

// TODO: WIP WIP WIP WIP
void Utilities::processPayload(const std::string payload) {

    // std::vector<std::string> strings;
    std::string temp(payload);

    int commandID{idCommand(temp)};

    if(commandID <= 0){
        // NOT A COMMAND
    }

    if(std::any_of(std::begin(temp), std::end(temp), [](char c) { return c == ';'; })) {
        std::regex semiColon(";");
        std::regex_replace(temp, semiColon, ",");
    }
    
    auto strings{split(temp, ',')};

}

// PART: PRIVATE

int Utilities::idCommand(const std::string payload) {

    std::regex rx("^[\\w\\s]+(?=,)");
    std::smatch match;
    std::regex_search(payload, match, rx);

    if(!match.empty()){
        std::string command(match.str());
        std::transform(command.begin(), command.end(), command.begin(), [] (unsigned char c){ return std::toupper(c); });
        if(command == "SEND MSG") return 1;
        else if(command == "GET MSG") return 2;
        else if(command == "KEEPALIVE") return 3;
        else if(command == "CONNECT") return 4;
        else if(command == "LEAVE") return 5;
        else if(command == "STATUSREQ") return 6;
        else if(command == "LISTSERVERS") return 7;
        else if(command == "SERVERS") return 8;
        else if(command == "STATUSRESP") return 9;
        else return 0;
    } else return -1;
}


std::vector<std::string> Utilities::split(std::string stringToSplit, char delimeter) {
    std::stringstream ss(stringToSplit);
    std::string word;
	std::vector<std::string> splittedStrings;
    while (std::getline(ss, word, delimeter)) splittedStrings.push_back(word);
    return splittedStrings;
}