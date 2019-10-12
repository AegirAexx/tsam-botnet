// TODO: Make constructor.

#include "../../include/model/Client.h"

Client::Client(int sock, std::string name) {
    // this->commandID = commandID;
    // memset (this->buffer, 0, 4096);
    // strcpy (this->buffer, arguments.c_str());
    // this->argCount = 0;
}

Client::~Client() {}

// Add a pretty print overload
std::ostream& operator << (std::ostream& outs, const Client& client){
    Client tempCommand = client;
    outs << "Client: " << client.name << std::endl;
    return outs;
}
