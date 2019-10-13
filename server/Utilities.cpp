
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

// TODO: FINISH VALIDATION
// FEAT: Right now we are checking for the presents of SOH and EOT.
// FEAT: Check for the location of 0xHexes?
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

bool Utilities::validateCommand(Command cmd) {
    if(cmd.getID() == 4) isCONNECT(cmd.getPayload());
    else if (cmd.getID() != 1) return false;
    else return false;
}
bool Utilities::isCONNECT(std::vector<std::string> payload) {
    return false;
}

// PART: Non workable code. LEGACY.

// int Utilities::idCommand(const char *buffer) {

//     std::string payload(buffer);
//     std::regex rx("^[\\w\\s]+(?=,)");
//     std::smatch match;
//     std::regex_search(payload, match, rx);

//     if(!match.empty()){
//         std::string command(match.str());
//         std::transform(command.begin(), command.end(), command.begin(), [] (unsigned char c){ return std::toupper(c); });
//         if(command == "SEND_MSG") return 1;
//         else if(command == "GET_MSG") return 2;
//         else if(command == "KEEPALIVE") return 3;
//         else if(command == "CONNECT") return 4;
//         else if(command == "LEAVE") return 5;
//         else if(command == "STATUSREQ") return 6;
//         else if(command == "LISTSERVERS") return 7;
//         else if(command == "SERVERS") return 8;
//         else if(command == "STATUSRESP") return 9;
//         else return 0;
//     } else return -1;
// }

// std::vector<std::string> Utilities::split(std::string stringToSplit, char delimeter) {
//     std::stringstream ss(stringToSplit);
//     std::string word;
// 	std::vector<std::string> splittedStrings;
//     while (std::getline(ss, word, delimeter)) splittedStrings.push_back(word);
//     return splittedStrings;
// }

// SECTION: NETWORKING SERVER

// void Utilities::closeClient(std::vector<Client> clients, int clientSocket, fd_set *openSockets, int *maxfds) {

//     // Remove client from the clients list
//     // BUG: Change the data structure from std::map => std::vector
//     // clients.erase(clientSocket);


//     if(*maxfds == clientSocket) {
//         for(auto const& p : clients) {
//             // *maxfds = std::max(*maxfds, p.second->sock); // BUG: Missing reference to the parameter in the client data structure.
//         }
//     }

//     // And remove from the list of open sockets.
//     FD_CLR(clientSocket, openSockets);
// }

// // SECTION: NETWORKING SERVER

// void Utilities::clientCommand(std::vector<Client> clients, int clientSocket, fd_set *openSockets, int *maxfds, char *buffer) {

//     int command{idCommand(buffer)};

//     if(command > 0) {
//         if(command == 1){

//         } else if(command == 2){

//         } else if(command == 3){

//         } else if(command == 4){

//         } else if(command == 5){

//         } else if(command == 6){

//         } else if(command == 7){

//         } else if(command == 8){

//         } else if(command == 9){

//         }
//     } else {
//         // Somthing went wrong(LAYER 8).
//     }
// }
//     // PART: FROM TEACHER: BUFFER
//     std::vector<std::string> tokens;
//     std::string token;
//     std::string msg;
//     std::stringstream stream(buffer);
//     while(stream >> token) tokens.push_back(token);
// }

// // DEBUG:
//  void Utilities::listCommands() {
//     std::cout << "############ KNOWN COMMANDS ############" << std::endl;
//     std::cout << "~ NO COMMAND - #(-1)" << std::endl;
//     std::cout << "~ UNKNOWN COMMAND - MADE IT THROUGH THE FILTER - COMMAND #(0)" << std::endl;
//     std::cout << "SEND MSG - COMMAND #(1)" << std::endl;
//     std::cout << "GET MSG - COMMAND #(2)" << std::endl;
//     std::cout << "KEEPALIVE - COMMAND #(3)" << std::endl;
//     std::cout << "CONNECT - COMMAND #(4)" << std::endl;
//     std::cout << "LEAVE - COMMAND #(5)" << std::endl;
//     std::cout << "STATUSREQ - COMMAND #(6)" << std::endl;
//     std::cout << "LISTSERVERS - COMMAND #(7)" << std::endl;
//     std::cout << "########################################" << std::endl;
// }




// std::vector<Command> Utilities::processPayload(const std::string payload) {

//     std::vector<std::string> args;
//     std::string temp(payload);
//     int commandID{idCommand(temp.c_str())};
//     std::vector<Command> commands;

//     if(commandID > 0) {
//         if(std::any_of(std::begin(temp), std::end(temp), [](char c) { return c == ';'; })) {
//             std::regex rx(",");
//             args = split(temp, ';');
//             std::smatch match;
//             std::regex_search(args.at(0), match, rx);
//             args.at(0) = match.suffix();
//             for(size_t i {0}; i < args.size(); ++i){
//                 Command command(commandID, args[i]);
//                 commands.push_back(command);
//             }
//         } else {
//             args = split(temp, ',');
//             args.erase(args.begin());
//             std::string tempSingle;
//             for(auto i : args) {
//                 tempSingle += (i + ",");
//             }
//             tempSingle.pop_back();
//             Command command(commandID, tempSingle);
//             commands.push_back(command);
//         }
//     }
//     return commands;
// }