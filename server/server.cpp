#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include "Utilities.h"


//Backlog

class Server {
  public:
    int sock;              // socket of client connection
    std::string name;           // Limit length of name of client's user
    std::string ipAddress;
    int port;
    bool isCOC = false;
    Server(int socket) : sock(socket) {}
    ~Server() {}
};

std::map<int, Server*> servers;
int serverCount{0};


void closeServer(int serverSocket, fd_set *openSockets, int *maxfds) {
    // Remove client from the clients list

    if(!servers[serverSocket]->isCOC) {
        serverCount--;
    }
    servers.erase(serverSocket);

    // If this client's socket is maxfds then the next lowest
    // one has to be determined. Socket fds can be reused by the Kernel,
    // so there aren't any nice ways to do this.

    if(*maxfds == serverSocket) {
        for(auto const& p : servers) {
            *maxfds = std::max(*maxfds, p.second->sock);
        }
    }

    // And remove from the list of open sockets.
    FD_CLR(serverSocket, openSockets);
}



void connectToServer(std::string ipAddress, int port, fd_set *openSockets, int *maxfds, std::string name) {

    // COM: Create a socket
    int serverSock{socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)};

    if(serverSock < 0) {
        std::perror("Can't create socket");
        return;
    }

    //Socket address
    struct sockaddr_in sk_addr;
    memset(&sk_addr, 0, sizeof(sk_addr));

    sk_addr.sin_family = AF_INET;
    sk_addr.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &sk_addr.sin_addr);



    // COM: Connect to another server
    int connectResponse{connect(serverSock, (sockaddr*)&sk_addr, sizeof(sockaddr_in))};

    if(connectResponse < 0) {
        std::perror("Can't connect");
        return;
    }

    // COM: Command set up



    FD_SET(serverSock, &*openSockets);

    *maxfds = std::max(*maxfds, serverSock);
    // // create a new client to store information.



    servers[serverSock] = new Server(serverSock);

    std::cout <<  "ipAddress: " << ipAddress << std::endl;
    std::cout <<  "port: " << port << std::endl;

    // DAGUR: store ipAddress and port number of new connection
    servers[serverSock]->name = name;
    servers[serverSock]->ipAddress = ipAddress;
    servers[serverSock]->port = port;
}

// Process command from client on the server

void clientCommand(int clientSocket, fd_set *openSockets, int *maxfds, char *buffer, std::string myIpAddress, int myPort) {
    std::vector<std::string> tokens;
    std::string token;
    std::string msg;
    // Split command from client into tokens for parsing
    std::stringstream stream(buffer);
    std::string group("P3_GROUP_4");
    Utilities u;

    while(stream >> token) tokens.push_back(token);

    if((tokens[0].compare("CONNECT") == 0) && (tokens.size() == 4)) { //Usage: CONNECT groupID IPaddress Port
        //Connect to client
        // TODO: need to find the right place for the naming of the client, should not be here I think
        //clients[clientSocket]->name = "client"; //Naming of client should not be here
        connectToServer(tokens[2], atoi(tokens[3].c_str()), &*openSockets, &*maxfds, tokens[1]);
        msg = "Si patron";
        send(clientSocket, msg.c_str(), msg.length(), 0);  // DAGUR: Tok ut -1 af msg.length ???
    } else if(tokens[0].compare("LEAVE") == 0) {
        // Close the socket, and leave the socket handling
        // code to deal with tidying up clients etc. when
        // select() detects the OS has torn down the connection.
        closeServer(clientSocket, openSockets, maxfds);
        msg = "Hasta la vista baby!";
        send(clientSocket, msg.c_str(), msg.length()-1, 0);
    } else if(tokens[0].compare("LISTSERVERS") == 0) {

        //Get IP address, put in message to send
        msg += "SERVERS," + group + "," + myIpAddress + "," + std::to_string(myPort) + ";";
        //Go through clients/servers map and add all to message
        for(auto const& client : servers) {
            //char port = static_cast<char>(client.second->port);
            msg += client.second->name + "," + client.second->ipAddress + "," +  std::to_string(client.second->port) + ";";
        }
        //Add start & end hex
        std::string formattedMsg(u.rebuildString(msg));
        send(clientSocket, formattedMsg.c_str(), formattedMsg.length(), 0);
    } else if((tokens[0].compare("MSG") == 0) && (tokens[1].compare("ALL") == 0)) {
        // This is slightly fragile, since it's relying on the order
        // of evaluation of the if statement.

        for(auto i = tokens.begin()+2;i != tokens.end();i++) {
            msg += *i + " ";
        }

        for(auto const& pair : servers) {
            send(pair.second->sock, msg.c_str(), msg.length(),0);
        }
    } else if(tokens[0].compare("MSG") == 0) {
        for(auto const& pair : servers) {
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

void serverCommand(int clientSocket, fd_set *openSockets, int *maxfds, char *buffer, std::string myIpAddress, int myPort) {
    std::vector<std::string> tokens;
    std::string token;
    std::string msg;
    // Split command from client into tokens for parsing
    std::stringstream stream(buffer);
    std::string group("P3_GROUP_4");
    Utilities u;

    while(stream >> token) tokens.push_back(token);

    if((tokens[0].compare("CONNECT") == 0) && (tokens.size() == 4)) { //Usage: CONNECT groupID IPaddress Port
        //Connect to client
        // TODO: need to find the right place for the naming of the client, should not be here I think
        //clients[clientSocket]->name = "client"; //Naming of client should not be here
        connectToServer(tokens[2], atoi(tokens[3].c_str()), &*openSockets, &*maxfds, tokens[1]);
        msg = "Si patron";
        send(clientSocket, msg.c_str(), msg.length(), 0);  // DAGUR: Tok ut -1 af msg.length ???
    } else if(tokens[0].compare("LEAVE") == 0) {
        // Close the socket, and leave the socket handling
        // code to deal with tidying up clients etc. when
        // select() detects the OS has torn down the connection.
        closeServer(clientSocket, openSockets, maxfds);
        msg = "Hasta la vista baby!";
        send(clientSocket, msg.c_str(), msg.length()-1, 0);
    } else if(tokens[0].compare("LISTSERVERS") == 0) {

        //Get IP address, put in message to send
        msg += "SERVERS," + group + "," + myIpAddress + "," + std::to_string(myPort) + ";";
        //Go through clients/servers map and add all to message
        for(auto const& client : servers) {
            //char port = static_cast<char>(client.second->port);
            msg += client.second->name + "," + client.second->ipAddress + "," +  std::to_string(client.second->port) + ";";
        }
        //Add start & end hex
        std::string formattedMsg(u.rebuildString(msg));
        send(clientSocket, formattedMsg.c_str(), formattedMsg.length(), 0);
    } else if((tokens[0].compare("MSG") == 0) && (tokens[1].compare("ALL") == 0)) {
        // This is slightly fragile, since it's relying on the order
        // of evaluation of the if statement.

        for(auto i = tokens.begin()+2;i != tokens.end();i++) {
            msg += *i + " ";
        }

        for(auto const& pair : servers) {
            send(pair.second->sock, msg.c_str(), msg.length(),0);
        }
    } else if(tokens[0].compare("MSG") == 0) {
        for(auto const& pair : servers) {
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


int main(int argc, char* argv[]){

    // COM: Open socket

    struct sockaddr_in server_addr; //Server socket address
    struct sockaddr_in client_addr; //Client socket address

    int serverSock{socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)};
    int clientSock{socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)};

    int set = 1;
    int serverPort{atoi(argv[1])};
    int clientPort{atoi(argv[2])};

    Utilities u; //Utilities class

    std::string myIpAddress(u.getLocalIP()); //Get our Ip address

    //struct to keep hold of incoming server address
    struct sockaddr_in server;
    struct sockaddr_in client;
    // DAGUR:Sennilega lengdin a client structinu
    socklen_t serverLen;
    socklen_t clientLen;

    if(serverSock < 0) {
        std::perror("Failed to open server socket");
        return -1;
    }

    if(clientSock < 0) {
        std::perror("Failed to open client socket");
        return -1;
    }

    // Set sock opt SO_REUSADDR // DAGUR: Einhverskonar hefd samkvaemt geeks for geeks
    if(setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(set)) < 0) {
        std::perror("Failed to set SO_REUSEADDR:");
    }

    if(setsockopt(clientSock, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(set)) < 0) {
        std::perror("Failed to set SO_REUSEADDR:");
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(serverPort);

    memset(&client_addr, 0, sizeof(client_addr));

    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = INADDR_ANY;
    client_addr.sin_port = htons(clientPort);

    // Bind to socket to listen for connections from clients
    if(bind(serverSock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::perror("Failed to bind to server socket:");
        return -1;
    }

    if(bind(clientSock, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        std::perror("Failed to bind to client socket:");
        return -1;
    }


    std::cout << "Listening for servers on socket#: " << serverSock << std::endl;
    std::cout << "Listening for clients on socket#: " << clientSock << std::endl;

    // COM: Now we need to listen

    //create Socket list
    fd_set openSockets;

    //Copies of openSockets will be stored here
    fd_set readSockets;
    fd_set exceptSockets;

    //listen
    if(listen(serverSock, 5) < 0) {
        std::perror("Listen failed on server port");
        return -1;
    }

    if(listen(clientSock, 0) < 0) {
        std::perror("Listen failed on client port");
        return -1;
    }

    //Add listen socket to socket set we are monitoring
    FD_SET(serverSock, &openSockets);
    FD_SET(clientSock, &openSockets);

    //Maxfds
    int maxfds{clientSock};

    //Socket to take care of new connections
    int newSock{0};


    bool isFinished{false};
    char buffer[1024];

    while(!isFinished) {
        //Get modifiable copy of openSockets
        readSockets = exceptSockets = openSockets;
        memset(buffer, 0, sizeof(buffer));

        int n = select(maxfds + 1, &readSockets, NULL, &exceptSockets, NULL);

        if(n < 0) {
            std::perror("select failed - closing down\n");
            isFinished = true;
            continue;
        }

        // Add listen socket to socket set we are monitoring
        if(FD_ISSET(clientSock, &readSockets)) { //Maybe some boolean check to see if it is already connected
            newSock = accept(clientSock, (struct sockaddr *)&client, &clientLen);
            // Add new client to the list of open sockets
            FD_SET(newSock, &openSockets);
            // And update the maximum file descriptor
            maxfds = std::max(maxfds, newSock);
            // MAybe some C&C info if we want.
            servers[newSock] = new Server(newSock);
            servers[newSock]->isCOC = true;
            // Decrement the number of sockets waiting to be dealt with
            n--;
            std::cout << "Client connected on socket: " << newSock << std::endl;
        }

        if(FD_ISSET(serverSock, &readSockets)&& servers.size() < 6) {
            newSock = accept(serverSock, (struct sockaddr *)&server, &serverLen);
            // Add new client to the list of open sockets
            FD_SET(newSock, &openSockets);
            // And update the maximum file descriptor
            maxfds = std::max(maxfds, newSock);
            // create a new client to store information.
            servers[newSock] = new Server(newSock);
            serverCount++; //increment server count
            // Decrement the number of sockets waiting to be dealt with
            n--;
            std::cout << "Server connected on socket: " << newSock << std::endl;
        }

        while(n > 0) {
            for(auto const& pair : servers) {
                Server *server = pair.second;
                if(FD_ISSET(server->sock, &readSockets)) {
                    // recv() == 0 means client has closed connection
                    if(recv(server->sock, buffer, sizeof(buffer), MSG_DONTWAIT) == 0) {
                        std::cout << "Server or client closed connection:" << server->sock << std::endl;
                        close(server->sock);
                        closeServer(server->sock, &openSockets, &maxfds);
                        break; // DAGUR: Added this so server would not crash when connection is closed
                    } else {
                        // We don't check for -1 (nothing received) because select()
                        // only triggers if there is something on the socket for us.
                        std::cout << buffer << std::endl;
                        //IF client then client command else servercommand
                        if(server->isCOC) {
                            clientCommand(server->sock, &openSockets, &maxfds, buffer, myIpAddress, serverPort);
                        }
                        else {
                            serverCommand(server->sock, &openSockets, &maxfds, buffer, myIpAddress, serverPort);
                        }

                    }
                }
            }
            n--;
        }
    }

    return 0;
}


