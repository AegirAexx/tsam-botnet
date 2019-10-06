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


//Backlog

class Client {
  public:
    int sock;              // socket of client connection
    std::string name;           // Limit length of name of client's user
    std::string ipAddress;
    int port;
    Client(int socket) : sock(socket) {}
    ~Client() {}
};

std::map<int, Client*> clients;


void closeClient(int clientSocket, fd_set *openSockets, int *maxfds) {
    // Remove client from the clients list
    clients.erase(clientSocket);

    // If this client's socket is maxfds then the next lowest
    // one has to be determined. Socket fds can be reused by the Kernel,
    // so there aren't any nice ways to do this.

    if(*maxfds == clientSocket) {
        for(auto const& p : clients) {
            *maxfds = std::max(*maxfds, p.second->sock);
        }
    }

    // And remove from the list of open sockets.
    FD_CLR(clientSocket, openSockets);
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



    clients[serverSock] = new Client(serverSock);

    std::cout <<  "ipAddress: " << ipAddress << std::endl;
    std::cout <<  "port: " << port << std::endl;

    // DAGUR: store ipAddress and port number of new connection
    clients[serverSock]->name = name;
    clients[serverSock]->ipAddress = ipAddress;
    clients[serverSock]->port = port;
}

// Process command from client on the server

void clientCommand(int clientSocket, fd_set *openSockets, int *maxfds, char *buffer) {
    std::vector<std::string> tokens;
    std::string token;
    std::string msg;
    // Split command from client into tokens for parsing
    std::stringstream stream(buffer);

    while(stream >> token) tokens.push_back(token);

    if((tokens[0].compare("CONNECT") == 0) && (tokens.size() == 4)) { //Usage: CONNECT groupID IPaddress Port
        //Connect to client
        // TODO: need to find the right place for the naming of the client, should not be here I think
        clients[clientSocket]->name = "client"; //Naming of client should not be here
        connectToServer(tokens[2], atoi(tokens[3].c_str()), &*openSockets, &*maxfds, tokens[1]);
        msg = "Si patron";
        send(clientSocket, msg.c_str(), msg.length(), 0);  // DAGUR: Tok ut -1 af msg.length ???
    } else if(tokens[0].compare("LEAVE") == 0) {
        // Close the socket, and leave the socket handling
        // code to deal with tidying up clients etc. when
        // select() detects the OS has torn down the connection.
        closeClient(clientSocket, openSockets, maxfds);
        msg = "Hasta la vista baby!";
        send(clientSocket, msg.c_str(), msg.length()-1, 0);
    } else if(tokens[0].compare("WHO") == 0) {
        std::cout << "Who is logged on" << std::endl;
        // TODO: Breyta thessu i skipunina LISTSERVER og byrja a ad na i eigin ip addressu og port og prenta svo ut client mapid
        for(auto const& client : clients) {
            //char port = static_cast<char>(client.second->port);
            msg += client.second->name + " " + "ip: " + client.second->ipAddress + " port: "; //+ port + ","; // DAGUR: Prenta ut ipAddress og port a theim sem eru tengdir
        }
        // Reducing the msg length by 1 loses the excess "," - which // DAGUR: Eg breytti thessu lika, er ekki ad eyda sidasta stakinu
        // granted is totally cheating.
        send(clientSocket, msg.c_str(), msg.length(), 0);
    } else if((tokens[0].compare("MSG") == 0) && (tokens[1].compare("ALL") == 0)) {
        // This is slightly fragile, since it's relying on the order
        // of evaluation of the if statement.

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


int main(int argc, char* argv[]){

    // COM: Open socket

    struct sockaddr_in sk_addr;
    int sock{socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)};
    int set = 1;
    int portno{atoi(argv[1])};

    //struct to keep hold of incoming client address
    struct sockaddr_in client;
    // DAGUR:Sennilega lengdin a client structinu
    socklen_t clientLen;

    if(sock < 0) {
        std::perror("Failed to open socket");
        return -1;
    }

    // Set sock opt SO_REUSADDR // DAGUR: Einhverskonar hefd samkvaemt geeks for geeks
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(set)) < 0) {
        std::perror("Failed to set SO_REUSEADDR:");
    }

    memset(&sk_addr, 0, sizeof(sk_addr));

    sk_addr.sin_family = AF_INET;
    sk_addr.sin_addr.s_addr = INADDR_ANY;
    sk_addr.sin_port = htons(portno);

    // Bind to socket to listen for connections from clients
    if(bind(sock, (struct sockaddr *)&sk_addr, sizeof(sk_addr)) < 0) {
        std::perror("Failed to bind to socket:");
        return -1;
    }

    int listenSock{sock};
    int clientSock{0};

    std::cout << "Listening on socket#: " << listenSock << std::endl;

    // COM: Now we need to listen

    //create Socket list
    fd_set openSockets;

    //Copies of openSockets will be stored here
    fd_set readSockets;
    fd_set exceptSockets;

    //listen
    if(listen(listenSock, 5) < 0) {
        std::perror("Listen failed on port");
        return -1;
    }

    //Add listen socket to socket set we are monitoring
    FD_SET(listenSock, &openSockets);

    //Maxfds
    int maxfds{listenSock};


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
        if(FD_ISSET(listenSock, &readSockets)) {
            clientSock = accept(listenSock, (struct sockaddr *)&client, &clientLen);
            // Add new client to the list of open sockets
            FD_SET(clientSock, &openSockets);
            // And update the maximum file descriptor
            maxfds = std::max(maxfds, clientSock);
            // create a new client to store information.
            clients[clientSock] = new Client(clientSock);
            // Decrement the number of sockets waiting to be dealt with
            n--;
            std::cout << "Client connected on socket: " << clientSock << std::endl;
        }

        while(n > 0) {
            for(auto const& pair : clients) {
                Client *client = pair.second;
                if(FD_ISSET(client->sock, &readSockets)) {
                    // recv() == 0 means client has closed connection
                    if(recv(client->sock, buffer, sizeof(buffer), MSG_DONTWAIT) == 0) {
                        std::cout << "Client closed connection:" << client->sock << std::endl;
                        close(client->sock);
                        closeClient(client->sock, &openSockets, &maxfds);
                        break; // DAGUR: Added this so server would not crash when connection is closed
                    } else {
                        // We don't check for -1 (nothing received) because select()
                        // only triggers if there is something on the socket for us.
                        std::cout << buffer << std::endl;
                        clientCommand(client->sock, &openSockets, &maxfds, buffer);
                    }
                }
            }
            n--;
        }
    }

    return 0;
}


