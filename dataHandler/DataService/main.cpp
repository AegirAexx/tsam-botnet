// SECTION: All datatypes in the mix.

#include "DataService.h"
#include "SentMessage.h"
#include "ReceivedMessage.h"
#include "ServerProfile.h"

#include <fstream>
#include <iostream>
#include <vector>


int main(){


    std::string payload("The goal is to link your server and client into a class wide botnet, and use them to fulfill the assignment todo list. \n\nIn order to do that, you will need to give some thought to routing commands through the network, connecting to and leaving the botnet, storing messages for disconnected servers, message expiry, and handling commands for other groups. There is also a mysterious Oracle to be found.\n\nIn particular, remember that there is other information besides that purely in the messages for example, knowing which link to your server the message arrived on.");

    std::cout << "ReceivedMessage..." << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;


    // COM: Received - Two constructors
    ReceivedMessage messageR1("V_GROUP_122", "V_GROUP_134", payload, 1569710743);
    ReceivedMessage messageR2;
    std::cout << messageR1;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << messageR2;
    std::cout << "--------------------------------------------------------------------" << std::endl;

    std::cout << "\nSentMessage..." << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;


    // COM: Sent - Two constructors
    SentMessage messageS1("V_GROUP_12", payload, 9710743);
    SentMessage messageS2;
    std::cout << messageS1;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << messageS2;
    std::cout << "--------------------------------------------------------------------" << std::endl;

    std::cout << "\nServerProfile..." << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;


    // COM: Server datatype - Two constructors
    ServerProfile server1(4084, "123.245.130.251", "V_GROUP_111");
    ServerProfile server2(4067, "133.225.120.261", "V_GROUP_121");
    ServerProfile server3(4012, "153.255.110.271", "V_GROUP_131");
    ServerProfile server4;

    std::cout << server1;
    std::cout << server2;
    std::cout << server3;
    std::cout << server4;
    std::cout << "--------------------------------------------------------------------" << std::endl;

// TODO: Write to a data file.

    // PART: INPUT FROM FILE

    // std::vector<std::string> logs;
    // std::string lineLog;
    // std::ifstream messageLogs;
    // messageLogs.open("logs.txt");
    // while (std::getline(messageLogs, lineLog)){
    //     logs.push_back(lineLog);
    // }
    // messageLogs.close();

    // PART: OUTPUT TO FILE

    return 0;
}