#include "Server.h"

Server::Server(int socket) {
    this->sock = socket;
    this->port = 0;
    this->name = "nullptr";
    this->ipAddress = "nullptr";
    this->isCOC = false;
}

int Server::getSocket() {
    return this->sock;
}

int Server::getPort() {
    return this->port;
}

std::string Server::getName() {
    return this->name;
}

std::string Server::getIpAddress() {
    return this->ipAddress;
}

bool Server::getIsCOC() {
    return this->isCOC;
}