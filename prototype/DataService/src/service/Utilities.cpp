
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

    // LISTSERVERS,<FROM_GROUP_ID>
    // KEEPALIVE,<# of Messages>
    // GET MSG,<GROUP_ID>
    // SEND MSG,<FROM_GROUP_ID>,<TO_GROUP_ID>,<Message content>
    // LEAVE,SERVER_IP,PORT
    // STATUSREQ,FROM_GROUP
    // CONNECT ??

int Utilities::idCommand(std::string payload){

    // !isComma?
    // if(!std::any_of(std::begin(payload), std::end(payload), [=](char c) { return c == ','; })) {
    //     // std::cout << "no " << ',' << std::endl;
    //     // ERROR
    // }

    // ((SEND|GET)[\s-_]?MSG|KEEPALIVE|CONNECT|LEAVE|STATUSREQ|LISTSERVERS)

    // std::regex rx("^[A-Za-z\s-_]+(?=,)");
    std::regex rx("^[A-Za-z\\s]+(?=,)");
    std::smatch match;
    std::regex_search(payload, match, rx);

    if(!match.empty()){
        std::string command(match.str());
        std::transform(command.begin(), command.end(), command.begin(), [] (unsigned char c){ return std::toupper(c); });
        if(command == "SEND MSG"){
            std::cout << "SEND MSG - REGISTERED - COMMAND #1 " + match.str() << std::endl;
            return 1;
        } else if(command == "GET MSG"){
            std::cout << "GET MSG - REGISTERED - COMMAND #2 " + match.str() << std::endl;
            return 2;
        } else if(command == "KEEPALIVE"){
            std::cout << "KEEPALIVE - REGISTERED - COMMAND #3 " + match.str() << std::endl;
            return 3;
        } else if(command == "CONNECT"){
            std::cout << "CONNECT - REGISTERED - COMMAND #4 " + match.str() << std::endl;
            return 4;
        } else if(command == "LEAVE"){
            std::cout << "LEAVE - REGISTERED - COMMAND #5 " + match.str() << std::endl;
            return 5;
        } else if(command == "STATUSREQ"){
            std::cout << "STATUSREQ - REGISTERED - COMMAND #6 " + match.str() << std::endl;
            return 6;
        } else if(command == "LISTSERVERS"){
            std::cout << "LISTSERVERS - REGISTERED - COMMAND #7 " + match.str() << std::endl;
            return 7;
        } else {
            std::cout << "UNKNOWN COMMAND - MADE IT THROUGH THE FILTER" << std::endl;
        }

    }
    std::cout << "No command found in payload" << std::endl;
    return -1;
 }


// PART: PRIVATE

std::vector<std::string> Utilities::split(std::string stringToSplit, char delimeter) {
    std::stringstream ss(stringToSplit);
    std::string word;
	std::vector<std::string> splittedStrings;
    while (std::getline(ss, word, delimeter)) splittedStrings.push_back(word);
    return splittedStrings;
}