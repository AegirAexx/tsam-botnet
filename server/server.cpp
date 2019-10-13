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

//Backlog

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

// DAGUR: Sloppy hornid
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
    // so there aren't any nice ways to do this.

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

    Utilities u;
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

    servers[serverSock] = new Server(serverSock);

    std::cout <<  "ipAddress: " << ipAddress << std::endl; // DEBUG:
    std::cout <<  "port: " << port << std::endl; // DEBUG:

    // DAGUR: store ipAddress and port number of new connection
    servers[serverSock]->name = name;
    auto temp = u.split(name, '_');
    servers[serverSock]->groupID = atoi(temp[temp.size()-1].c_str());
    servers[serverSock]->ipAddress = ipAddress;
    servers[serverSock]->port = port;
}

void clientCommand(int clientSocket, fd_set *openSockets, int *maxfds, char *buffer, std::string myIpAddress, int myPort) {

    //Jacky code
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream stream(buffer);



    // COM: Split command from client into tokens for parsing
    std::string msg;
    Utilities u;
    tokens = u.split(buffer, ',');

    Command c(buffer);

    std::cout << "Command #: " << c.getID() << std::endl; // DEBUG:

    std::cout << "Test"  << std::endl; // DEBUG:

    std::cout << "Command buffer: " << buffer << std::endl; // DEBUG:
    // or(auto i : c.getPayload()) std::cout << i;

    if(c.getID() == 4) { // COM: CONNECT
        // DAGUR: We need to take this command out later as it is not in the specifications, but good to have during development

        connectToServer(c.getPayload()[1], atoi(c.getPayload()[2].c_str()), &*openSockets, &*maxfds, c.getPayload()[0]);
        msg = "Si patron";
        send(clientSocket, msg.c_str(), msg.length(), 0);
    } else if(c.getID() == 7) { // COM: LISTSERVERS
        //Go through clients/servers map and add all to message
        for(auto const& server : servers) {
            msg += server.second->name + "," + server.second->ipAddress + "," +  std::to_string(server.second->port) + ";";
        }
        //Add start & end hex
        // DEBUG: Tharf thetta tegar madur er ad respond-a a client, sennilega ekki
        std::string formattedMsg(u.addRawBytes(msg));
        send(clientSocket, formattedMsg.c_str(), formattedMsg.length(), 0);
    }
    else if(c.getID() == 10) { // COM: Client GETMSG

        // COM: Get group ID
        auto temp = u.split(c.getPayload()[0], '_');
        int groupID = atoi(temp[temp.size()-1].c_str());

        // Athuga fjolda skilaboda i array
        int msgCount = groupMsgCount[groupID];

        if(msgCount < 1) {
            // Eg engin tha "Sorry no messages for you, to GET_MSG use XXXX format"
            msg = "Sorry, there are no messages for " + c.getPayload()[0] + " in our system: Protocol to get message from server is 'GETMSG,<P3_GROUP_X>' where X is group number";

            std::string formattedMsg(u.addRawBytes(msg));
            send(clientSocket, formattedMsg.c_str(), formattedMsg.length(), 0);
        }
        else {
            // Annars rulla i gegnum queue og finna skilabodin og pussla theim saman
            auto it = msgQ.begin();

            for(unsigned int i = 0; i < msgQ.size(); i++) {
                if(msgQ[i].getTo() == c.getPayload()[0]) {
                    msg = msgQ[i].getFrom() + "," + msgQ[i].getTo() + "," + msgQ[i].getMsg();

                    std::string formattedMsg(u.addRawBytes(msg));
                    send(clientSocket, formattedMsg.c_str(), formattedMsg.length(), 0);

                    //Message log
                    //msgQ[i].logMessage();

                    //Decrement group message count
                    groupMsgCount[groupID]--;

                    //Remove message from deque
                    msgQ.erase(it);

                    // TODO: Herna sennilega break
                }
                it++;
            }
        }
    } else if(tokens[0].compare("SENDMSG") == 0) { // COM: SENDMSG

        // COM: Create new Message to store message info
        // Message *newMessage = new Message(c);

        std::cout << "Start creating newMessage" << std::endl;

        std::string from = group;
        std::string to = c.getPayload()[0];
        std::string msg;

        // std::string from = group;
        // std::string to = tokens[1];
        // std::string msg("test msg for debugging");

        for(unsigned int i = 1; i < c.getPayload().size(); i++) {
            msg += c.getPayload()[i] + " ";
        }

        std::cout << "From: " << from << " To: " << to << " msg: " << msg << std::endl; // DEBUG:

        msg.pop_back();

        Message *newMessage = new Message(from, to , msg);

        std::cout << "Created newMessage; From: " << newMessage->getFrom() << "To: " << newMessage->getTo() << " Msg: " << newMessage->getMsg() << std::endl;
        // COM: Add new message to FIFO data structure
        msgQ.push_back(*newMessage);
        std::cout << "Pushed back message to msgQ:" << std::endl;
        // DAGUR: Baeta herna inn ++ a videigandi array holf
        int groupID = msgQ.back().getGroupID();

        groupMsgCount[groupID] = groupMsgCount[groupID] + 1; //increment count of messages for group
        // DAGUR: Delete here?

        // Check if FIFO grind is full or index[0] msg is too old
        if(msgQ.size() > 10) { // msgQ.front().getTimeStamp()
            // then send to random one-hopper

        }

        // Send confirmation to client
        msg = "Message stored in Q, From: " + msgQ.back().getFrom() + " To: " + msgQ.back().getTo() + " Message: " + msgQ.back().getMsg() + " With group ID: " + std::to_string(msgQ.back().getGroupID());
        std::string formattedMsg(u.addRawBytes(msg));
        send(clientSocket, formattedMsg.c_str(), formattedMsg.length(), 0);

        //Log
        //msgQ.back().logMessage();
    } else if(c.getID() == 5) { //LEAVE
        // Tokens[1] = serverIp
        std::string ipAddressToLeave(c.getPayload()[0]); // COM: Tharf ad breyta i c.payload eitthvad
        // Tokens[2] = serverPort
        int portToLeave{atoi(c.getPayload()[1].c_str())};

        // DAGUR: Gaeti madur ekki i raun bara checkad hvort server.second->sock == serverSocket ???
        for(auto const& server : servers) {
            if((server.second->ipAddress == ipAddressToLeave) && (server.second->port == portToLeave)) {
                std::cout << "Adios " << server.second->name << std::endl;
                closeServer(server.second->sock, openSockets, maxfds);
                break;
            }
        }
    } else {
        std::cout << "Unknown command from client:" << buffer << std::endl;
    }
}

void serverCommand(int serverSocket, fd_set *openSockets, int *maxfds, char *buffer, std::string myIpAddress, int myPort) {
    Utilities u;

    // COM: Clean SOH & EOT from buffer
    // TODO: Need to validate message somewhere to see if it has SOH & EOT before we clean it, also if we have noise after or before SOH and EOT I think we are supposed to ignore it
    auto cleanBuffer = u.removeRawBytes(buffer); // DAGUR: FEAT: removeRawBytes() already partially validates.

    // COM: Split command from client into tokens for parsing
    //auto tokens = u.split(cleanBuffer, ',');

    Command c(cleanBuffer);

    // String to hold response message
    std::string msg;

    if(c.getID() == 7) { // DAGUR: LISTSERVERS
        // Get IP address, put in message to send
        msg += "SERVERS," + group + "," + myIpAddress + "," + std::to_string(myPort) + ";";
        //Go through clients/servers map and add all to message

        for(auto const& server : servers) {
            // If server is not a client then add to msg // DAGUR: er thessi isCOC logic kannski sma stupid? aettum vid ad hafa ser client map frekar?
            if(!server.second->isCOC) {
                msg += server.second->name + "," + server.second->ipAddress + "," +  std::to_string(server.second->port) + ";";
            }
        }
        //Add start & end hex
        std::string formattedMsg(u.addRawBytes(msg));
        send(serverSocket, formattedMsg.c_str(), formattedMsg.length(), 0);
    } else if(c.getID() == 5) {  // LEAVE  DAGUR: Ran into trouble debugging, tharf ad baeta inn client megin for debugging purposes
        // Tokens[1] = serverIp
        std::string ipAddressToLeave(c.getPayload()[0]); // COM: Tharf ad breyta i c.payload eitthvad
        // Tokens[2] = serverPort
        int portToLeave{atoi(c.getPayload()[1].c_str())};

        // DAGUR: Gaeti madur ekki i raun bara checkad hvort server.second->sock == serverSocket ???
        for(auto const& server : servers) {
            if((server.second->ipAddress == ipAddressToLeave) && (server.second->port == portToLeave)) {
                std::cout << "Adios " << server.second->name << std::endl;
                closeServer(server.second->sock, openSockets, maxfds);
            }
        }
    } else if(c.getID() == 3) { // COM: KEEPALIVE
        // Hvernig a eiginlega ad virka?
        // Taka vid keepalive skilabod-um fra odrum server, meta hvort tad seu einhver skilabod til okkar
        // t.e. ad fjoldinn se staerri en 0, ef svo er tha a ad gera get msg a vidkomandi socket

        //Thegar vid faum keepAlive i okkur tha aetlum vid ad update-a keepAlive timan
        size_t temp = u.getTimestamp();
        servers[serverSocket]->lastKeepAlive = temp;
        //Cout out-a thad for debugging purposes
        std::cout << "Updated keep alive time for " << servers[serverSocket]->name << std::endl;

        if(atoi(c.getPayload()[c.getPayload().size()-1].c_str()) > 0) {

            msg = "GET_MSG," + group;

            //Add start & end hex
            std::string formattedMsg(u.addRawBytes(msg));
            send(serverSocket, formattedMsg.c_str(), formattedMsg.length(), 0);
        }

    } else if(c.getID() == 2) { // COM: GET_MSG

        // COM: Get group ID
        auto temp = u.split(c.getPayload()[0], '_');
        int groupID = atoi(temp[temp.size()-1].c_str());

        // Athuga fjolda skilaboda i array
        int msgCount = groupMsgCount[groupID];

        if(msgCount < 1) {
            // Eg engin tha "Sorry no messages for you, to GET_MSG use XXXX format"
            msg = "Sorry, there are no messages for " + c.getPayload()[0] + " in our system: Protocol to get message from server is 'GETMSG,<P3_GROUP_X>' where X is group number";

            std::string formattedMsg(u.addRawBytes(msg));
            send(serverSocket, formattedMsg.c_str(), formattedMsg.length(), 0);
        }
        else {
            // Annars rulla i gegnum queue og finna skilabodin og pussla theim saman
            auto it = msgQ.begin();

            for(unsigned int i = 0; i < msgQ.size(); i++) {
                if(msgQ[i].getTo() == c.getPayload()[0]) {
                    msg = "SEND_MSG," + msgQ[i].getFrom() + "," + msgQ[i].getTo() + "," + msgQ[i].getMsg();

                    std::string formattedMsg(u.addRawBytes(msg));
                    send(serverSocket, formattedMsg.c_str(), formattedMsg.length(), 0);

                    //Message log
                    //msgQ[i].logMessage();

                    //Decrement group message count
                    groupMsgCount[groupID]--;

                    //Remove message from deque
                    msgQ.erase(it);

                    // TODO: Sennilega break her svo ekki oll skilabodin seu send
                }
                it++;
            }
        }
    } else if(c.getID() == 1) { // COM: SEND_MSG

        // COM: Create new Message to store message info
        // Message *newMessage = new Message(c);

        // // COM: Add new message to FIFO data structure
        // msgQ.push_back(*newMessage);

        // // DAGUR: Baeta herna inn ++ a videigandi array holf
        // int groupID = msgQ.back().getGroupID();

        // groupMsgCount[groupID] = groupMsgCount[groupID] + 1; //increment count of messages for group
        // // DAGUR: Delete here?

        // // Check if FIFO grind is full or index[0] msg is too old
        // if(msgQ.size() > 10) { // msgQ.front().getTimeStamp()
        //     // then send to random one-hopper

        // }
        //Log
        //msgQ.back().logMessage();
    } else if (c.getID() == 8){ // COM: SERVERS

        servers[serverSocket]->name = c.getPayload()[0];

        auto temp = u.split(c.getPayload()[0], '_');
        servers[serverSocket]->groupID =atoi(temp[temp.size()-1].c_str());
        std::cout << "GroupID a incoming server connection: " << servers[serverSocket]->groupID << std::endl;
        // TODO: Aetti ad vera haegt ad skra allar thessar upplysingar fyrir utan nafnid a theim timapunkti sem accept() er gert
        servers[serverSocket]->ipAddress = c.getPayload()[1];
        servers[serverSocket]->port = atoi(c.getPayload()[2].c_str());

        // TODO: Svo tharf ad dila vid restina af strengnum sem inniheldur upplysingar um alla onehop dudes
        std::cout << "SERVERS message from server:" << cleanBuffer << std::endl; // DEBUG:

        // Taka alla server-ana i svarinu og athuga fyrst hvort vid seum nu thegar tengdir theim
        // Ef ekki tha geyma tha i timabundinni gagnagrind (map svipad og servers vaentanlega)
        // rulla svo i gegnum tha og gera connect a server-ana a medan ad servercount er minna en 5

    } else if(c.getID() == 6) { // COM: STATUSREQ
        // Thegar thetta kemur inn tha a madur ad skila tilbaka streng sem byrjar a STATUSRESP
        // Og svo fara ofan i gagnagrindina og finna oll skilabodin sem madur er med thar
        // Held eg en frekar oskyrt
    } else if(c.getID() == 9) { // COM: STATUSRESP
        // Thegar thetta kemur inn tydir ad madur er ad fa response fra statusreq sem madur hefur sent
        // Madur vaentanlega sendir statusreq til ad ga hvada skilabod onehop gaurarnir manns eru med
        // Svo er tha spurning hvad madur gerir vid thessar upplysingar?
        // Fer vaentanlega eftir tvi hvernig madur utfaerir get MSG
    } else {
        std::cout << "Unknown command from server: " << buffer << std::endl;
        std::cout << "Command #: " << c.getID() << std::endl;
    }
}

size_t sendKeepAlive() {
    Utilities u;
    std::string aliveMsg("KEEPALIVE,");

    for(auto const& pair : servers) {
        Server *server = pair.second;

        // DAGUR: "std::to_chars" virkar kannski betur til ad stoppa auka null i strengnum?
        // DAGUR: Vildi fokka i tessu en tetta gaeti virkad.
        if(!server->isCOC) {
            aliveMsg += std::to_string(groupMsgCount[server->groupID]); // DAGUR: Herna thyrfti ad finna ut fjolda skilaboda sem vidkomandi a i message menginu okkar

            std::string formattedMsg(u.addRawBytes(aliveMsg));
            send(server->sock, formattedMsg.c_str(), formattedMsg.length(), 0);

            std::cout << "Send keep alive to: " << server->name << " With groupID: " << server->groupID << " With count: " << groupMsgCount[server->groupID] << std::endl; // DEBUG:
        }
    }

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

    // DAGUR:Sennilega lengdin a client structinu
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


    std::cout << "Listening for servers on socket#: " << serverSock << std::endl; // DEBUG:
    std::cout << "Listening for clients on socket#: " << clientSock << std::endl; // DEBUG:

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

    struct timeval tv{60, 0};
    size_t lastKeepAlive = u.getTimestamp(); //Initialize-a med now
    size_t interruptTime{0};

    while(!isFinished) {
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
            servers[newSock] = new Server(newSock); // DAGUR: Afhverju erum vid aftur ad skra client-ana inn i map-id????
            servers[newSock]->isCOC = true;
            // Decrement the number of sockets waiting to be dealt with
            n--;
            std::cout << "Client connected on socket: " << newSock << std::endl; // DEBUG:
        }

        if(FD_ISSET(serverSock, &readSockets)&& servers.size() < 5) {
            newSock = accept(serverSock, (struct sockaddr *)&server, &serverLen);
            // Add new client to the list of open sockets
            FD_SET(newSock, &openSockets);
            // And update the maximum file descriptor
            maxfds = std::max(maxfds, newSock);
            // create a new client to store information.
            servers[newSock] = new Server(newSock);
            // DAGUR: Herna aettum vid ad geta skrad inn upplysingarnar um thann sem er ad tengjast okkur strax med server.sin_addr og server.sin_port
            // Thar eru upplysingarnar thurfum i raun ekki ad bida eftir LSTSERVER >> SERVER svarinu
            // TODO: add ipAddress and port to new Server .... server.sin_ipAdress server.sin_port
            // DAGUR: Sma tilraunamennska her
            // FEAT: https://stackoverflow.com/a/9212542
            // FEAT: https://stackoverflow.com/questions/37721310/c-how-to-get-ip-and-port-from-struct-sockaddr
            // servers[serverSock]->ipAddress = server.sin_addr.s_addr;
            // servers[serverSock]->port = server.sin_port;
            // std::cout << "Sin address: " << server.sin_addr.s_addr << std::endl;
            // std::cout << "Sin port: " << server.sin_port << std::endl;

            serverCount++; //increment server count
            std::cout << "Server count: " << serverCount << std::endl;
            //Listserver sent to incoming connection server
            std::string msg;
            msg = u.handshake(group, myIpAddress, serverPort);
            send(newSock, msg.c_str(), msg.length(), 0);
            // Decrement the number of sockets waiting to be dealt with
            n--;
            std::cout << "Server connected on socket: " << newSock << std::endl; // DEBUG:
        }

        if(n == 0) { //Vid timeout-udum
            lastKeepAlive = sendKeepAlive(); //update-a lastKeepAlive
            tv.tv_sec = 60;
        }
        else {
            interruptTime = u.getTimestamp();
            tv.tv_sec = interruptTime - lastKeepAlive;
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


