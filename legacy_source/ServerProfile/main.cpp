#include "ServerProfile.h"
#include <iostream>



int main() {
    ServerProfile server1(4084, "123.245.130.251", "V_GROUP_111");
    ServerProfile server2(4067, "133.225.120.261", "V_GROUP_121");
    ServerProfile server3(4012, "153.255.110.271", "V_GROUP_131");
    ServerProfile server4;

    std::cout << server1;
    std::cout << server2;
    std::cout << server3;
    std::cout << server4;

    return 0;
}