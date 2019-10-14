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
#include <queue>
#include <deque>
#include "Utilities.h"
#include "Message.h"

class Server {
  public:
    int sock;              // socket of client connection
    std::string name;           // Limit length of name of client's user
    std::string ipAddress;
    int groupID;
    int port;
    bool isCOC = false;
    size_t lastKeepAlive;
    Server(int socket) : sock(socket) {}
    ~Server() {}
};

// Global variables
std::map<int, Server*> servers;
int serverCount{0};
std::string group("P3_GROUP_4");
std::deque<Message> msgQ;
int groupMsgCount[200]{0};


void closeServer(int serverSocket, fd_set *openSockets, int *maxfds) {
    // Remove client from the clients list
    if(!servers[serverSocket]->isCOC) {
        std::cout << "Server count decremented " << std::endl;
        serverCount--;
    }
    std::cout << "Erasing server sockets " << std::endl;
    servers.erase(serverSocket);

    // If this client's socket is maxfds then the next lowest
    // one has to be determined. Socket fds can be reused by the Kernel,
    std::cout << "New max fds " << std::endl;
    if(*maxfds == serverSocket) {
        for(auto const& p : servers) {
            *maxfds = std::max(*maxfds, p.second->sock);
        }
    }
    // And remove from the list of open sockets.
    std::cout << "Remove from the list of open sockets " << std::endl;
    FD_CLR(serverSocket, openSockets);
    std::cout << "Removed succesfully from the list of open sockets " << std::endl;
}

void connectToServer(std::string ipAddress, int port, fd_set *openSockets, int *maxfds, std::string name) {
    //Initialize Utilities class for helper functions
    Utilities u;
    //Create a socket
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

    //Connect to another server
    int connectResponse{connect(serverSock, (sockaddr*)&sk_addr, sizeof(sockaddr_in))};

    if(connectResponse < 0) {
        std::perror("Can't connect");
        return;
    }
    //Command set up
    FD_SET(serverSock, &*openSockets);

    *maxfds = std::max(*maxfds, serverSock);

    servers[serverSock] = new Server(serverSock);

    std::cout <<  "ipAddress: " << ipAddress << std::endl; // DEBUG:
    std::cout <<  "port: " << port << std::endl; // DEBUG:

    // Store ipAddress and port number of new connection
    servers[serverSock]->name = name;
    auto temp = u.split(name, '_');
    servers[serverSock]->groupID = atoi(temp[temp.size()-1].c_str());
    servers[serverSock]->ipAddress = ipAddress;
    servers[serverSock]->port = port;

    //Send listservers to new connection
    std::string msg;
    msg = u.handshake(group);
    send(serverSock, msg.c_str(), msg.length(), 0);
}

void clientCommand(int clientSocket, fd_set *openSockets, int *maxfds, char *buffer, std::string myIpAddress, int myPort) {
    //Initialize Utilities class for helper functions
    Utilities u;
    // String to hold response message
    std::string msg;
    //Splits buffer and checks which kind of command we are receiving
    Command c(buffer);

    if(c.getID() == 4) { //CONNECT function to make server connect to other servers
        if(u.validateCommand(c)) {
            connectToServer(c.getPayload()[1], atoi(c.getPayload()[2].c_str()), &*openSockets, &*maxfds, c.getPayload()[0]);
            msg = "Si patron";
            send(clientSocket, msg.c_str(), msg.length(), 0);
        } else {
            msg = "Invalid usage of CONNECT please format correctly, usage: CONNECT,<Group name>,<ipAddress>,<port>";
            send(clientSocket, msg.c_str(), msg.length(), 0);
        }
    } else if(c.getID() == 7) { //LISTSERVERS
        if(u.validateCommand(c)) {
            //Go through clients/servers map and add all to message
            for(auto const& server : servers) {
                msg += server.second->name + "," + server.second->ipAddress + "," +  std::to_string(server.second->port) + ";";
            }
            //Add start & end hex
            std::string formattedMsg(u.addRawBytes(msg));
            send(clientSocket, formattedMsg.c_str(), formattedMsg.length(), 0);
        } else {
            msg = "Invalid usage of LISTSERVERS please format correctly, usage: LISTSERVERS,<Group name>";
            send(clientSocket, msg.c_str(), msg.length(), 0);
        }
    }
    else if(c.getID() == 10) { //Client GETMSG
        if(u.validateCommand(c)) {
            //Get group ID
            auto temp = u.split(c.getPayload()[0], '_');
            int groupID = atoi(temp[temp.size()-1].c_str());

            // Check number of messages in array
            int msgCount = groupMsgCount[groupID];

            if(msgCount < 1) {
                //If no messages for group, send that info to client
                msg = "Sorry, there are no messages for " + c.getPayload()[0] + " in our system: Protocol to get message from server is 'GETMSG,<P3_GROUP_X>' where X is group number";

                std::string formattedMsg(u.addRawBytes(msg));
                send(clientSocket, formattedMsg.c_str(), formattedMsg.length(), 0);
            }
            else {
                //Otherwise find messages in message queue
                auto it = msgQ.begin();

                for(unsigned int i = 0; i < msgQ.size(); i++) {
                    if(msgQ[i].getTo() == c.getPayload()[0]) {
                        msg = msgQ[i].getFrom() + "," + msgQ[i].getTo() + "," + msgQ[i].getMsg();
                        std::string formattedMsg(u.addRawBytes(msg));
                        send(clientSocket, formattedMsg.c_str(), formattedMsg.length(), 0);
                        //Message log
                        msgQ[i].logMessage(c.getID());
                        //Decrement group message count
                        groupMsgCount[groupID]--;
                        //Remove message from deque
                        msgQ.erase(it);
                    }
                    it++;
                }
            }
        } else {
            msg = "Invalid usage of GETMSG please format correctly, usage: GETMSG,<Group name>";
            send(clientSocket, msg.c_str(), msg.length(), 0);
        }
    } else if(c.getID() == 11) { //SENDMSG
        if(u.validateCommand(c)) {
            // Create new Message to store message info
            std::string from = group;
            std::string to = c.getPayload()[0];
            std::string msg;

            for(unsigned int i = 1; i < c.getPayload().size(); i++) {
                msg += c.getPayload()[i] + " ";
            }

            std::cout << "From: " << from << " To: " << to << " msg: " << msg << std::endl;

            //Remove trailing " "
            msg.pop_back();

            Message *newMessage = new Message(from, to , msg);

            std::cout << "Created newMessage; From: " << newMessage->getFrom() << "To: " << newMessage->getTo() << " Msg: " << newMessage->getMsg() << std::endl;
            // Add new message to FIFO data structure
            msgQ.push_back(*newMessage);
            std::cout << "Pushed back message to msgQ:" << std::endl;
            //Get groupID
            int groupID = msgQ.back().getGroupID();
            //increment count of messages for group
            groupMsgCount[groupID] = groupMsgCount[groupID] + 1;

            // Message expiry system, check if FIFO structure is full
            if(msgQ.size() > 10) {
                //Then erease from system
                std::cout << "Message expired, erase from structure" << std::endl;
                msgQ.pop_front();
            }

            // Send confirmation to client
            msg = "Message stored in Q, From: " + msgQ.back().getFrom() + " To: " + msgQ.back().getTo() + " Message: " + msgQ.back().getMsg() + " With group ID: " + std::to_string(msgQ.back().getGroupID());
            std::string formattedMsg(u.addRawBytes(msg));
            send(clientSocket, formattedMsg.c_str(), formattedMsg.length(), 0);

            //Log
            msgQ.back().logMessage(c.getID());
        } else {
            msg = "Invalid usage of SENDMSG please format correctly, usage: SENDMSG,<Group name to>,<message>";
            send(clientSocket, msg.c_str(), msg.length(), 0);
        }
    } else if(c.getID() == 5) { //LEAVE
        if(u.validateCommand(c)) {
            //ipAddress to leave
            std::string ipAddressToLeave(c.getPayload()[0]);
            //port to leave
            int portToLeave{atoi(c.getPayload()[1].c_str())};

            //Find server to leave
            for(auto const& server : servers) {
                if((server.second->ipAddress == ipAddressToLeave) && (server.second->port == portToLeave)) {
                    std::cout << "Adios " << server.second->name << std::endl;
                    //Disconnect from server
                    closeServer(server.second->sock, openSockets, maxfds);
                    break;
                }
            }
        } else {
            msg = "Invalid usage of LEAVE please format correctly, usage: LEAVE,<ipAddress>,<port>";
            send(clientSocket, msg.c_str(), msg.length(), 0);
        }
    } else {
        std::cout << "Unknown command from client:" << buffer << std::endl;
    }
}

void serverCommand(int serverSocket, fd_set *openSockets, int *maxfds, char *buffer, std::string myIpAddress, int myPort) {
    //Initialize Utilities class for helper functions
    Utilities u;
    // COM: Clean SOH & EOT from buffer
    auto cleanBuffer = u.removeRawBytes(buffer);
    //Splits buffer and checks which kind of command we are receiving
    Command c(cleanBuffer);
    // String to hold response message
    std::string msg;

    if(c.getID() == 7) { //LISTSERVERS
        if(u.validateCommand(c)) {
            // Get IP address, put in message to send
            msg = "SERVERS," + group + "," + myIpAddress + "," + std::to_string(myPort) + ";";

            //Go through clients/servers map and add all to message
            for(auto const& server : servers) {
                // If server is not a client then add to msg
                if(!server.second->isCOC) {
                    msg += server.second->name + "," + server.second->ipAddress + "," +  std::to_string(server.second->port) + ";";
                }
            }
            //Add start & end hex
            std::string formattedMsg(u.addRawBytes(msg));
            send(serverSocket, formattedMsg.c_str(), formattedMsg.length(), 0);
        } else {
            msg = "Invalid usage of LISTSERVERS please format correctly, usage: LISTSERVERS,<Group name>";
            send(serverSocket, msg.c_str(), msg.length(), 0);
        }
    } else if(c.getID() == 5) {  // LEAVE
        if(u.validateCommand(c)) {
            std::string ipAddressToLeave(c.getPayload()[0]);
            int portToLeave{atoi(c.getPayload()[1].c_str())};

            //Finna hvern a ad aftengjast vid
            for(auto const& server : servers) {
                if((server.second->ipAddress == ipAddressToLeave) && (server.second->port == portToLeave)) {
                    std::cout << "Adios " << server.second->name << std::endl;
                    closeServer(server.second->sock, openSockets, maxfds);
                }
            }
        } else {
            msg = "Invalid usage of LEAVE please format correctly, usage: LEAVE,<ipAddress>,<port>";
            send(serverSocket, msg.c_str(), msg.length(), 0);
        }
    } else if(c.getID() == 3) { //KEEPALIVE
        if(u.validateCommand(c)) {
            //Update the last time we received keep alive
            size_t temp = u.getTimestamp();
            servers[serverSocket]->lastKeepAlive = temp;

            //If keepalive is indicating we have a message we send GET_MSG
            if(atoi(c.getPayload()[c.getPayload().size()-1].c_str()) > 0) {
                msg = "GET_MSG," + group;
                //Add start & end hex
                std::string formattedMsg(u.addRawBytes(msg));
                send(serverSocket, formattedMsg.c_str(), formattedMsg.length(), 0);
            }
        } else {
            msg = "Invalid usage of KEEPALIVE please format correctly, usage: KEEPALIVE,<# of messages>";
            send(serverSocket, msg.c_str(), msg.length(), 0);
        }
    } else if(c.getID() == 2) { //GET_MSG
        if(u.validateCommand(c)) {
            //Get group ID
            auto temp = u.split(c.getPayload()[0], '_');
            int groupID = atoi(temp[temp.size()-1].c_str());
            //Check number of messages for group
            int msgCount = groupMsgCount[groupID];
            //If group has messages we go through the queue and send the group all messages for given groupID
            if(msgCount > 0) {
                auto it = msgQ.begin();

                for(unsigned int i = 0; i < msgQ.size(); i++) {
                    if(msgQ[i].getTo() == c.getPayload()[0]) {
                        msg = "SEND_MSG," + msgQ[i].getFrom() + "," + msgQ[i].getTo() + "," + msgQ[i].getMsg();

                        //Add start & end
                        std::string formattedMsg(u.addRawBytes(msg));
                        send(serverSocket, formattedMsg.c_str(), formattedMsg.length(), 0);

                        //Message log
                        msgQ[i].logMessage(c.getID());

                        //Decrement group message count
                        groupMsgCount[groupID]--;

                        //Remove message from deque
                        msgQ.erase(it);
                    }
                    it++;
                }
            }
        } else {
            msg = "Invalid usage of GET_MSG please format correctly, usage: GET_MSG,<Group name>";
            send(serverSocket, msg.c_str(), msg.length(), 0);
        }
    } else if(c.getID() == 1) { //SEND_MSG
        if(u.validateCommand(c)) {
            //Create new Message to store message info
            std::string from = c.getPayload()[0];
            std::string to = c.getPayload()[1];
            std::string msg;

            for(unsigned int i = 2; i < c.getPayload().size(); i++) {
                msg += c.getPayload()[i] + " ";
            }
            std::cout << "From: " << from << " To: " << to << " msg: " << msg << std::endl;
            //Remove trailing " "
            msg.pop_back();
            //Message constructor
            Message *newMessage = new Message(from, to , msg);
            std::cout << "Created newMessage; From: " << newMessage->getFrom() << "To: " << newMessage->getTo() << " Msg: " << newMessage->getMsg() << std::endl;
            //Add new message to FIFO data structure
            msgQ.push_back(*newMessage);
            std::cout << "Pushed back message to msgQ:" << std::endl;
            // Get groupID
            int groupID = msgQ.back().getGroupID();
            //increment count of messages for group
            groupMsgCount[groupID] = groupMsgCount[groupID] + 1;

            // Message expiry system, check if FIFO structure is full
            if(msgQ.size() > 10) {
                //Then erease from system
                std::cout << "Message expired, erase from structure" << std::endl;
                msgQ.pop_front();
            }
            // Send confirmation to server
            msg = "Message stored in Q, From: " + msgQ.back().getFrom() + " To: " + msgQ.back().getTo() + " Message: " + msgQ.back().getMsg() + " With group ID: " + std::to_string(msgQ.back().getGroupID());
            std::string formattedMsg(u.addRawBytes(msg));
            send(serverSocket, formattedMsg.c_str(), formattedMsg.length(), 0);

            //Log
            msgQ.back().logMessage(c.getID());
        } else {
            msg = "Invalid usage of SEND_MSG please format correctly, usage: SEND_MSG,<Group name from>,<Group name to>,<message>";
            send(serverSocket, msg.c_str(), msg.length(), 0);
        }
    } else if (c.getID() == 8) { //SERVERS
        if(u.validateCommand(c)) {
            //Register group name for incoming connection
            servers[serverSocket]->name = c.getPayload()[0];
            //Split to get group ID, then register rest of group info
            auto temp = u.split(c.getPayload()[0], '_');
            servers[serverSocket]->groupID =atoi(temp[temp.size()-1].c_str());
            std::cout << "GroupID of incoming server connection: " << servers[serverSocket]->groupID << std::endl;
            servers[serverSocket]->ipAddress = c.getPayload()[1];
            servers[serverSocket]->port = atoi(c.getPayload()[2].c_str());

            std::cout << "Message:" << cleanBuffer << std::endl;
        } else {
            msg = "Invalid usage of CONNECT please format correctly, usage: CONNECT,<Group name>,<ipAddress>,<port>";
            send(serverSocket, msg.c_str(), msg.length(), 0);
        }

        // TODO:
        // Taka alla server-ana i svarinu og athuga fyrst hvort vid seum nu thegar tengdir theim
        // Ef ekki tha geyma tha i timabundinni gagnagrind (map svipad og servers vaentanlega)
        // rulla svo i gegnum tha og gera connect a server-ana a medan ad servercount er minna en 5

    } else if(c.getID() == 6) { //STATUSREQ
        if(u.validateCommand(c)) {
            // Reply with STATUSRESP
            msg = "STATUSRESP," + group + "," + c.getPayload()[0];

            //Check who is connected to us and how many messages we have for them
            for(auto const& server : servers) {
                msg += "," + server.second->name + "," + std::to_string(groupMsgCount[server.second->groupID]);
            }

            std::string formattedMsg(u.addRawBytes(msg));
            send(serverSocket, formattedMsg.c_str(), formattedMsg.length(), 0);
            std::cout << "Sending status req to " << c.getPayload()[0] << std::endl;
        } else {
            msg = "Invalid usage of CONNECT please format correctly, usage: CONNECT,<Group name>,<ipAddress>,<port>";
            send(serverSocket, msg.c_str(), msg.length(), 0);
        }
    } else if(c.getID() == 9) { //STATUSRESP
        //Print to screen status response
        std::cout << "Received STATUSRESP from " << c.getPayload()[0] << std::endl;
    } else {
        std::cout << "Unknown command from server: " << buffer << std::endl;
        std::cout << "Command #: " << c.getID() << std::endl;
    }
}

size_t sendKeepAlive() {
    //Initialize Utilities class for helper functions
    Utilities u;
    //KeepAlive message
    std::string aliveMsg("KEEPALIVE,");

    for(auto const& pair : servers) {
        Server *server = pair.second;

        if(!server->isCOC) {
            aliveMsg += std::to_string(groupMsgCount[server->groupID]);

            std::string formattedMsg(u.addRawBytes(aliveMsg));
            send(server->sock, formattedMsg.c_str(), formattedMsg.length(), 0);

            std::cout << "Sending keep alive to: " << server->name << " With count: " << groupMsgCount[server->groupID] << std::endl;
        }
    }
    //Timestamp to store last time KeepAlive was sent
    return u.getTimestamp();
}


int main(int argc, char* argv[]){

    // COM: Open socket
    struct sockaddr_in server_addr; //Server socket address
    struct sockaddr_in client_addr; //Client socket address

    //struct to keep hold of incoming server address
    struct sockaddr_in server;
    struct sockaddr_in client;

    int serverSock{socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)};
    int clientSock{socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)};

    int set = 1;
    int serverPort{atoi(argv[1])};
    int clientPort{atoi(argv[2])};

    Utilities u; //Utilities class

    std::string myIpAddress(u.getLocalIP()); //Get our Ip address

    socklen_t serverLen{0};
    socklen_t clientLen{0};

    if(serverSock < 0) {
        std::perror("Failed to open server socket");
        return -1;
    }

    if(clientSock < 0) {
        std::perror("Failed to open client socket");
        return -1;
    }

    // Set sock opt SO_REUSADDR
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

    std::cout << "Listening for servers on socket#: " << serverPort << std::endl; // DEBUG:
    std::cout << "Listening for clients on socket#: " << clientPort << std::endl; // DEBUG:

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

    size_t lastKeepAlive = u.getTimestamp(); //Initialize-a med now
    // size_t interruptTime{0};
    size_t now{0};

    //int newTime{60};

    while(!isFinished) {
        //Initialize timestruct 30, Keep alive sent on 60 - 90 second interval
        struct timeval tv{30, 0};

        now = u.getTimestamp();

        if((now - lastKeepAlive) > 60) {
            lastKeepAlive = sendKeepAlive();
        }

        // Memset
        memset(&server, 0, sizeof(server));
        memset(&client, 0, sizeof(client));

        //Get modifiable copy of openSockets
        readSockets = exceptSockets = openSockets;
        memset(buffer, 0, sizeof(buffer));

        int n = select(maxfds + 1, &readSockets, NULL, &exceptSockets, &tv);

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

        if(FD_ISSET(serverSock, &readSockets)&& servers.size() < 5) {
            newSock = accept(serverSock, (struct sockaddr *)&server, &serverLen);
            // Add new client to the list of open sockets
            FD_SET(newSock, &openSockets);
            // And update the maximum file descriptor
            maxfds = std::max(maxfds, newSock);
            // create a new client to store information.
            servers[newSock] = new Server(newSock);
            serverCount++; //increment server count
            //Listserver sent to incoming connection server
            std::string msg;
            msg = u.handshake(group);
            send(newSock, msg.c_str(), msg.length(), 0);
            // Decrement the number of sockets waiting to be dealt with
            n--;
            std::cout << "Server connected on socket: " << newSock << std::endl; // DEBUG:
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
                        break;
                    } else {
                        // We don't check for -1 (nothing received) because select()
                        // only triggers if there is something on the socket for us.
                        std::cout << buffer << std::endl;
                        auto temp = u.split(buffer, 0x01);

                        for(unsigned int i = 0; i < temp.size(); i++) {
                            if(server->isCOC) {
                                clientCommand(server->sock, &openSockets, &maxfds, buffer, myIpAddress, serverPort);
                            }
                            else {
                                serverCommand(server->sock, &openSockets, &maxfds, buffer, myIpAddress, serverPort);
                            }
                        }
                    }
                }
            }
            n--;
        }
    }
    return 0;
}


