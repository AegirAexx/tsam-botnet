
#include "Command.h"

Command::Command(std::string buffer) {

    std::regex rx("^[\\w\\s]+(?=,)");
    std::smatch match;
    std::regex_search(buffer, match, rx);
    if(!match.empty()){
        std::string command(match.str());
        std::transform(command.begin(), command.end(), command.begin(), [] (unsigned char c){ return std::toupper(c); });
        if(command == "SEND_MSG") this->id = 1;
        else if(command == "GET_MSG") this->id = 2;
        else if(command == "KEEPALIVE") this->id = 3;
        else if(command == "CONNECT") this->id = 4;
        else if(command == "LEAVE") this->id = 5;
        else if(command == "STATUSREQ") this->id = 6;
        else if(command == "LISTSERVERS") this->id = 7;
        else if(command == "SERVERS") this->id = 8;
        else if(command == "STATUSRESP") this->id = 9;
        else if(command == "GETMSG") this->id = 10;
        else if(command == "SENDMSG") this->id = 11;
        else if(command == "CONNECT") this->id = 12;
        else this->id = 0;
    } else this->id = -1;



    if(this->id > 0) {
        std::regex rx(";");
        this->payload = split(std::regex_replace(buffer,rx,","), ',');
        this->payload.erase(this->payload.begin());
    }

}

std::vector<std::string> Command::split(std::string stringToSplit, char delimeter) {
    std::stringstream ss(stringToSplit);
    std::string word;
	std::vector<std::string> splittedStrings;
    while (std::getline(ss, word, delimeter)) splittedStrings.push_back(word);
    return splittedStrings;
}

std::vector<std::string>  Command::getPayload(){
    return this->payload;
}

int Command::getID(){
    return this->id;
}