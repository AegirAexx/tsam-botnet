#include <server.h>
#include <client.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <string.h>

#include <unistd.h>

#include <iostream>

constexpr int QUE {5};

// TODO: Data structure for clients - GLOBALLY?
// TODO: Data structure for servers - GLOBALLY?

int main(int argc, char* argv[]){

    // PART: INITIALIZATION

    // Hardcoded. Should be argv[2].
    int port{50000};

    // Create and initialize socket structure
    struct sockaddr_in socketAddress;
    memset(&socketAddress, 0, sizeof(socketAddress));

    // Set socket values
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    socketAddress.sin_port = htons(port);

    // Create and initialize buffer
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Create socket.
    int mainSocket{socket(AF_INET, SOCK_STREAM, 0)};
    int bindSocket{bind(mainSocket, (struct sockaddr *) &socketAddress, sizeof(socketAddress))};
    int listenSocket{listen(mainSocket, QUE)};

    int connectionSocket{0};

    // PART: VALIDATION

    // Check socket.
    if(mainSocket < 0) {
        std::perror("Failed to open socket");
        return(-1);
    }
    if(bindSocket < 0) {
        std::perror("Failed to bind socket");
        return(-1);
    }
    if (listenSocket < 0) {
        std::perror("Failed to listen socket");
        return(-1);
    }

    // PART: MAIN LOOP

    bool isFinished{false};

    while(!isFinished) {
        connectionSocket = accept(mainSocket, NULL, NULL);

        if(connectionSocket < 0) {
            std::perror("Failed to accept socket");
            return(-1);
        }

        // Readline(conn_s, buffer, MAX_LINE-1);
        // Writeline(conn_s, buffer, strlen(buffer));

        if(close(connectionSocket) < 0) {
            std::perror("Failed to close socket");
            return(-1);
        }
    }
    return 0;
}


