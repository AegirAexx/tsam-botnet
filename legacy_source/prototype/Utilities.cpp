
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


std::vector<Command> Utilities::processPayload(const std::string payload) {

    std::vector<std::string> args;
    std::string temp(payload);
    int commandID{idCommand(temp)};
    std::vector<Command> commands;

    if(commandID > 0) {
        if(std::any_of(std::begin(temp), std::end(temp), [](char c) { return c == ';'; })) {
            std::regex rx(",");
            args = split(temp, ';');
            std::smatch match;
            std::regex_search(args.at(0), match, rx);
            args.at(0) = match.suffix();
            for(size_t i {0}; i < args.size(); ++i){
                Command command(commandID, args[i]);
                commands.push_back(command);
            }
        } else {
            args = split(temp, ',');
            args.erase(args.begin());
            std::string tempSingle;
            for(auto i : args) {
                tempSingle += (i + ",");
            }
            tempSingle.pop_back();
            Command command(commandID, tempSingle);
            commands.push_back(command);
        }
    }
    return commands;
}


int Utilities::idCommand(const char *buffer) {

    std::string payload(buffer);
    std::regex rx("^[\\w\\s]+(?=,)");
    std::smatch match;
    std::regex_search(payload, match, rx);

    if(!match.empty()){
        std::string command(match.str());
        std::transform(command.begin(), command.end(), command.begin(), [] (unsigned char c){ return std::toupper(c); });
        if(command == "SEND_MSG") return 1;
        else if(command == "GET_MSG") return 2;
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

// SECTION: NETWORKING SERVER

void Utilities::closeClient(std::vector<Client> clients, int clientSocket, fd_set *openSockets, int *maxfds) {

    // Remove client from the clients list
    // BUG: Change the data structure from std::map => std::vector
    // clients.erase(clientSocket);


    if(*maxfds == clientSocket) {
        for(auto const& p : clients) {
            // *maxfds = std::max(*maxfds, p.second->sock); // BUG: Missing reference to the parameter in the client data structure.
        }
    }

    // And remove from the list of open sockets.
    FD_CLR(clientSocket, openSockets);
}

// SECTION: NETWORKING SERVER

void Utilities::clientCommand(std::vector<Client> clients, int clientSocket, fd_set *openSockets, int *maxfds, char *buffer) {

    int command{idCommand(buffer)};

    if(command > 0) {
        if(command == 1){

        } else if(command == 2){

        } else if(command == 3){

        } else if(command == 4){
            clients[clientSocket]->name = tokens[1];
            msg = "Connected!";
            send(clientSocket, msg.c_str(), msg.length()-1, 0);
        } else if(command == 5){

        } else if(command == 6){

        } else if(command == 7){

        } else if(command == 8){

        } else if(command == 9){

        }
    } else {
        // Somthing went wrong(LAYER 8).
    }

    // PART: FROM TEACHER: BUFFER
    std::vector<std::string> tokens;
    std::string token;
    std::string msg;
    std::stringstream stream(buffer);
    while(stream >> token) tokens.push_back(token);

    // PART: FROM TEACHER: COMMAND EXECUTION
    if((tokens[0].compare("CONNECT") == 0) && (tokens.size() == 2)) {
        clients[clientSocket]->name = tokens[1];
        msg = "Connected!";
        send(clientSocket, msg.c_str(), msg.length()-1, 0);
    } else if(tokens[0].compare("LEAVE") == 0) {
        closeClient(clientSocket, openSockets, maxfds);
        msg = "Hasta la vista baby!";
        send(clientSocket, msg.c_str(), msg.length()-1, 0);
    } else if(tokens[0].compare("WHO") == 0) {
        std::cout << "Who is logged on" << std::endl;

        for(auto const& names : clients) {
            msg += names.second->name + ",";
        }
        send(clientSocket, msg.c_str(), msg.length()-1, 0);
    } else if((tokens[0].compare("MSG") == 0) && (tokens[1].compare("ALL") == 0)) {
        for(auto i = tokens.begin()+2;i != tokens.end();i++) {
            msg += *i + " ";
        }

        for(auto const& pair : clients) {
            send(pair.second->sock, msg.c_str(), msg.length(),0);
        }
    } else if(tokens[0].compare("MSG") == 0) {
        for(auto const& pair : clients) {
            if(pair.second->name.compare(tokens[1]) == 0) {
                std::string msg;
                for(auto i = tokens.begin()+2;i != tokens.end();i++) {
                    msg += *i + " ";
                }
                send(pair.second->sock, msg.c_str(), msg.length(),0);
            }
        }
    } else {
        std::cout << "Unknown command from client:" << buffer << std::endl;
    }
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

