// SECTION: All datatypes in the mix.

#include "include/data/ServerProfileData.h"
#include "include/data/ReceivedMessageData.h"
#include "include/data/SentMessageData.h"
#include "include/model/SentMessage.h"
#include "include/model/ReceivedMessage.h"
#include "include/model/ServerProfile.h"
#include "include/service/Utilities.h"

#include <iostream>


int main(){

    // PART: DEMO MODEL DATATYPES
    // std::string payload("The goal is to link your server and client into a class wide botnet, and use them to fulfill the assignment todo list. \n\nIn order to do that, you will need to give some thought to routing commands through the network, connecting to and leaving the botnet, storing messages for disconnected servers, message expiry, and handling commands for other groups. There is also a mysterious Oracle to be found.\n\nIn particular, remember that there is other information besides that purely in the messages for example, knowing which link to your server the message arrived on.");

    // std::cout << "ReceivedMessage..." << std::endl;
    // std::cout << "--------------------------------------------------------------------" << std::endl;


    // // COM: Received - Two constructors
    // ReceivedMessage messageR1("V_GROUP_122", "V_GROUP_134", payload, 1569710743);
    // ReceivedMessage messageR2;
    // std::cout << messageR1;
    // std::cout << "--------------------------------------------------------------------" << std::endl;
    // std::cout << messageR2;
    // std::cout << "--------------------------------------------------------------------" << std::endl;

    // std::cout << "\nSentMessage..." << std::endl;
    // std::cout << "--------------------------------------------------------------------" << std::endl;


    // // COM: Sent - Two constructors
    // SentMessage messageS1("V_GROUP_12", payload, 9710743);
    // SentMessage messageS2;
    // std::cout << messageS1;
    // std::cout << "--------------------------------------------------------------------" << std::endl;
    // std::cout << messageS2;
    // std::cout << "--------------------------------------------------------------------" << std::endl;

    // std::cout << "\nServerProfile..." << std::endl;
    // std::cout << "--------------------------------------------------------------------" << std::endl;


    // // COM: Server datatype - Two constructors
    ServerProfile server1(4084, "123.245.130.251", "V_GROUP_111");
    // ServerProfile server2(4067, "133.225.120.261", "V_GROUP_121");
    // ServerProfile server3(4012, "153.255.110.271", "V_GROUP_131");
    // ServerProfile server4;

    std::cout << "Written from local: " << server1;
    // std::cout << "Written from local: " << server2;
    // std::cout << "Written from local: " << server3;
    // std::cout << "Written from local: " << server4;
    // std::cout << "--------------------------------------------------------------------" << std::endl;

    // COM: DataService layer - Member functions
    ServerProfileData data;
    std::cout << "writing data..." << std::endl;
    data.write(server1);
    std::cout << "writing data... DONE!" << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "printing data...\n" << std::endl;
    auto servers1 = data.getServers();
    for(auto i: servers1) std::cout << i;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "writing data..." << std::endl;
    data.write(server1);
    std::cout << "writing data... DONE!" << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "printing data...\n" << std::endl;
    auto servers2 = data.getServers();
    for(auto i: servers2) std::cout << i;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "writing data..." << std::endl;
    data.write(server1);
    std::cout << "writing data... DONE!" << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "printing data...\n" << std::endl;
    auto servers3 = data.getServers();
    for(auto i: servers3) std::cout << i;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "writing data..." << std::endl;
    data.write(server1);
    std::cout << "writing data... DONE!" << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "printing data...\n" << std::endl;
    auto servers4 = data.getServers();
    for(auto i: servers4) std::cout << i;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "writing data..." << std::endl;
    data.write(server1);
    std::cout << "writing data... DONE!" << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "printing data...\n" << std::endl;
    auto servers5 = data.getServers();
    for(auto i: servers5) std::cout << i;
    std::cout << "--------------------------------------------------------------------" << std::endl;


    // std::cout << std::boolalpha;
    // std::cout << "checkFile with true: " << data.checkFile(true) << std::endl;
    // std::cout << "--------------------------------------------------------------------" << std::endl;

    // // COM: Utility class
    // Utilities u;

    // std::cout << "seconds since Epoch: " << u.getTimestamp() << std::endl;
    // std::cout << "Local ip address: " << u.getLocalIP() << std::endl;

    // std::cout << "--------------------------------------------------------------------" << std::endl;

    // std::string command1("LISTSERVERS,V_GROUP_4");
    // std::string command2("KEEPALIVE,24");
    // std::string command3("GET MSG,V_GROUP_4");
    // std::string command4("SEND MSG,V_GROUP_4,V_GROUP_123,The cake is a lie! Do not trust GlaDos. Stay Safe.");
    // std::string command5("LEAVE," + u.getLocalIP() + ",21341");
    // std::string command6("STATUSREQ,V_GROUP_4");
    // std::string command7("CONNECT, V_GROUP_4");
    // std::string command8("TYPPI");
    // std::string command9("TYPPI,RASS");
    // std::string command0("SERVERS,V_GROUP_1,130.208.243.661,12344;V_GROUP_2,10.2.132.12,8888;V_GROUP_3,130.208.243.61,8878;V_GROUP_567,10.2.132.12,8128;V_GROUP_147,130.208.243.61,8458;V_GROUP_347,10.2.132.12,8848;");

    // std::cout << "--------------------------------------------------------------------" << std::endl;
    // u.listCommands();
    // std::cout << "--------------------------------------------------------------------" << std::endl;
    // auto a = u.processPayload(command4);
    // for(auto i : a) std::cout << i << std::endl;
    // std::cout << "--------------------------------------------------------------------" << std::endl;
    // auto b = u.processPayload(command6);
    // for(auto i : b) std::cout << i << std::endl;
    // std::cout << "--------------------------------------------------------------------" << std::endl;
    // auto c = u.processPayload(command2);
    // for(auto i : c) std::cout << i << std::endl;
    // std::cout << "--------------------------------------------------------------------" << std::endl;
    // auto d = u.processPayload(command9);
    // for(auto i : d) std::cout << i << std::endl;
    // std::cout << "--------------------------------------------------------------------" << std::endl;
    // auto e = u.processPayload(command1);
    // for(auto i : e) std::cout << i << std::endl;
    // std::cout << "--------------------------------------------------------------------" << std::endl;
    // auto f = u.processPayload(command3);
    // for(auto i : f) std::cout << i << std::endl;
    // std::cout << "--------------------------------------------------------------------" << std::endl;
    // auto g = u.processPayload(command8);
    // for(auto i : g) std::cout << i << std::endl;
    // std::cout << "--------------------------------------------------------------------" << std::endl;
    // auto h = u.processPayload(command0);
    // for(auto i : h) std::cout << i << std::endl;
    // std::cout << "--------------------------------------------------------------------" << std::endl;


    return 0;
}
