// SECTION: All datatypes in the mix.

#include "include/data/ServerProfileData.h"
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
    // ServerProfile server1(4084, "123.245.130.251", "V_GROUP_111");
    // ServerProfile server2(4067, "133.225.120.261", "V_GROUP_121");
    // ServerProfile server3(4012, "153.255.110.271", "V_GROUP_131");
    // ServerProfile server4;

    // std::cout << server1;
    // std::cout << server2;
    // std::cout << server3;
    // std::cout << server4;
    // std::cout << "--------------------------------------------------------------------" << std::endl;

    // // COM: DataService layer - Member functions
    // ServerProfileData data;

    // std::cout << "writing data..." << std::endl;

    // data.write(server1);
    // data.write(server2);
    // data.write(server3);
    // data.write(server4);

    // std::cout << "writing data... DONE!" << std::endl;

    // std::cout << "--------------------------------------------------------------------" << std::endl;

    // std::cout << "printing data...\n" << std::endl;

    // auto servers = data.getServers();
    // for(auto i: servers) std::cout << i;
    // std::cout << "--------------------------------------------------------------------" << std::endl;

    // std::cout << std::boolalpha;
    // std::cout << "checkFile with true: " << data.checkFile(true) << std::endl;
    // std::cout << "--------------------------------------------------------------------" << std::endl;

    // // COM: Utility class
    Utilities u;

    std::cout << "seconds since Epoch: " << u.getTimestamp() << std::endl;
    std::cout << "Local ip address: " << u.getLocalIP() << std::endl;
    std::cout << "--------------------------------------------------------------------" << std::endl;

    std::string command1("LISTSERVERS,V_GROUP_4");
    std::string command2("KEEPALIVE,24");
    std::string command3("GET MSG,V_GROUP_4");
    std::string command4("SEND MSG,V_GROUP_4,V_GROUP_123,The cake is a lie! Do not trust GlaDos. Stay Safe.");
    std::string command5("LEAVE," + u.getLocalIP() + ",21341");
    std::string command6("STATUSREQ,V_GROUP_4");
    std::string command7("CONNECT, V_GROUP_4");
    std::string command8("TYPPI");
    std::string command9("TYPPI,RASS");

    /* LISTSERVERS,V_GROUP_4 */
    /* KEEPALIVE,24 */
    /* GET MSG,V_GROUP_4 */
    /* SEND MSG,V_GROUP_4,V_GROUP_123,The cake is a lie! For real, do not trust GlaDos. Stay Safe. */
    /* LEAVE," + u.getLocalIP() + ",21341 */
    /* STATUSREQ,V_GROUP_4 */
    /* CONNECT, V_GROUP_4 */
    /* TYPPI */
    /* TYPPI,RASS */

    /* std::cout << "\nINPUT #1 (LISTSERVERS,V_GROUP_4): " << u.idCommand(command1) << std::endl; */
    /* std::cout << "\nINPUT #2 (KEEPALIVE,24): " << u.idCommand(command2) << std::endl; */
    /* std::cout << "\nINPUT #3 (GET MSG,V_GROUP_4): " << u.idCommand(command3) << std::endl; */
    /* std::cout << "\nINPUT #4 (SEND MSG,V_GROUP_4,V_GROUP_123,The cake is a lie! For real, do not trust GlaDos. Stay Safe.): " << u.idCommand(command4) << std::endl; */
    /* std::cout << "\nINPUT #5 (LEAVE," + u.getLocalIP() + ",21341): " << u.idCommand(command5) << std::endl; */
    /* std::cout << "\nINPUT #6 (STATUSREQ,V_GROUP_4): " << u.idCommand(command6) << std::endl; */
    /* std::cout << "\nINPUT #7 (CONNECT, V_GROUP_4): " << u.idCommand(command7) << std::endl; */
    /* std::cout << "\nINPUT #8 (TYPPI): " << u.idCommand(command8) << std::endl; */
    /* std::cout << "\nINPUT #9 (TYPPI,RASS): " << u.idCommand(command9) << std::endl; */

    std::cout << "--------------------------------------------------------------------" << std::endl;

    u.listCommands();

    std::cout << "--------------------------------------------------------------------" << std::endl;

    u.processPayload(command4);

    std::cout << "--------------------------------------------------------------------" << std::endl;


    return 0;
}
