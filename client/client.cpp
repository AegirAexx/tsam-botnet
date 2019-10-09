#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char* argv[]){

    // COM: Check if correct usage
    if(argc != 3) {
        std::cout << "usage: client <ip address> <ip port>\n";
        return 0;
    }

    // COM: Create a socket
    int sock{socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)};

    if(sock < 0) {
        std::perror("Can't create socket");
        return -1;
    }

    // TODO: Valid check args

    // COM: Variables for connection
    std::string ipAddress(argv[1]);
    int port{atoi(argv[2])};

    //Socket address
    sockaddr_in sk_addr;
    memset(&sk_addr, 0, sizeof(sk_addr));

    sk_addr.sin_family = AF_INET;
    sk_addr.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &sk_addr.sin_addr);

    // COM: Connect to the server

    int connectResponse{connect(sock, (sockaddr*)&sk_addr, sizeof(sockaddr_in))};

    if(connectResponse < 0) {
        std::perror("Can't connect");
        return -1;
    }

    // COM: Command set up

    bool isFinished{false};
    std::string command;
    char buffer[1024];


    while(!isFinished) {
        std::cout << "Enter command: ";
        getline(std::cin, command);

        // COM: Exit gracefully
        if(command == "exit") {
            isFinished = true;
            continue;
        }

        if(send(sock, command.c_str(), command.size(), 0) < 0) {
            std::perror("Can't send");
            return -1;
        }

        //Receive
        int in_packet{0};

        do{
            //Initalize memory
            memset(buffer, 0, 1024);
            //Get packet
            in_packet = recv(sock, buffer, 1024, 0);
            //Validate packet
            if(in_packet < 0) {
                std::perror("Can't receive");
                return -1;
            }
            //Write the buffer
            std::cout << buffer;

        } while(in_packet == 1024);
        // TODO: Check if it is neccessary to account for the last packet being exactly 1024
        std::cout << std::endl;
    }

    close(sock);

    return 0;
}