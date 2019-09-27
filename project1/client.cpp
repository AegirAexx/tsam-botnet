// Copyleft - aegir15@ru.is - Reykjavik University - 2019.
// Simple client for TSAM-409 Assignment 1

// Here I tried to shave as many unused libs as I could.
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]){

    // Check for correct usage.
    if(argc != 3){
        std::cout << "Usage: client [ip] [port]" << std::endl;
        exit(0);
    }

    // The program assumes that the user did everything correctly.
    // IRL you would need do better validation on the user input.
    // That is out of the scope of this project.

    // Create and initialize a socket
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int port = atoi(argv[2]);

    // Initialize socket object
    struct sockaddr_in sk_addr;
    sk_addr.sin_family = AF_INET;
    sk_addr.sin_port = htons(port);

    // Convert IP address from text to binary form. Takes the input string and an object reference.
    inet_pton(AF_INET, argv[1], &sk_addr.sin_addr);


    // Connect to server
    if(connect(sock, (sockaddr*)&sk_addr, sizeof(sockaddr_in)) < 0){
        perror("Failed to connect");
        return(-1);
    }

    // Create the message data variable.
    std::string message;

    // I was not sure about buffer size. Better safe than sorry?
    char res_server[8192];

    // Infinite loop that takes user input.
    // It does not have any way of ending gracefully.
    while(true){
        std::cout << "Client $> ";
        getline(std::cin, message);

        // Send
        if(send(sock, message.c_str(), message.size() + 1, 0) < 0){
            perror("Failed to send");
            return(-1);
        }

        // Initialise memory
        memset(res_server, 0, 8192);

        // Recieve
        if(recv(sock, res_server, 8192, 0) < 0){
            perror("Failed to recieve");
            return(-1);
        }

        // Write out the server response.
        std::cout << res_server << std::endl;
    }

    // Sadly the code will never get here because of the infinite loop.
    // Felt it should still be here if I ever fix the loop.
    close(sock);

    return 0;
}