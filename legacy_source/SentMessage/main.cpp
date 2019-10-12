#include "SentMessage.h"
#include <iostream>
#include <thread> // DEBUG TOOL - SLOW EVERYTHING




int main() {
    // PART: CHRONO TIME MANIPULATION
    // const auto timeStamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // std::cout << "Number of seconds since Epoch: " << (size_t)timeStamp << std::endl;
    // const auto timeStamp1 = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // const auto timeStamp2 = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    // const auto timeStamp3 = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // std::cout << "Timestamp1 " << (size_t)timeStamp1 << ";" << std::endl;
    // std::cout << "Timestamp2 " << (size_t)timeStamp2 << ";" << std::endl;
    // std::cout << "Timestamp3 " << (size_t)timeStamp3 << ";" << std::endl;

    // COM: Datatype demo.
    std::string payload("The goal is to link your server and client into a class wide botnet, and use them to fulfill the assignment todo list. \n\nIn order to do that, you will need to\t give some thought to routing commands through the network, connecting to and leaving the botnet, storing messages for disconnected servers, message expiry, and handling commands for other groups. There is also a mysterious Oracle to be found.\n\nIn particular, remember that there is other information besides that purely in the messages for example, knowing which link to your server the message arrived on.");

    SentMessage message("V_GROUP_12", payload, 1569710743);
    std::cout << message;


    return 0;
}