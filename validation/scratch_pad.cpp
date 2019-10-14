#include <iostream>
#include <fstream>
#include <regex>
#include <bitset>
#include <algorithm>
#include <vector>
#include <iterator>
#include <array>
#include <cctype>
#include <ctime>
#include <chrono>
#include "Command.h"
#include "Message.h"
#include "Utilities.h"

// PART: COMMANDS - MINIMUM REQUIRED
    // LISTSERVERS,<FROM_GROUP_ID>
    // KEEPALIVE,<# of Messages>
    // GET MSG,<GROUP_ID>
    // SEND MSG,<FROM_GROUP_ID>,<TO_GROUP_ID>,<Message content>
    // LEAVE,SERVER_IP,PORT
    // STATUSREQ,FROM_GROUP
    // CONNECT ??
// PART:

int main(int argc, char *argv[]){

    Utilities u;

    // DEBUG: FEAT: THESE ARE LEGIT
    // std::string commandstring1("SEND_MSG,P3_GROUP_4,P3_GROUP_1,The cake is a lie! Do not trust GlaDos. Stay Safe.");
    // std::string commandstring2("LISTSERVERS,P3_GROUP_12");
    // std::string commandstring3("GET_MSG,P3_GROUP_234");
    // std::string commandstring4("STATUSRESP,P3_GROUP_14,P3_GROUP_23,P3_GROUP_4,7,P3_GROUP_9,2");
    // std::string commandstring5("STATUSREQ,P3_GROUP_14");
    // std::string commandstring6("SERVERS,P3_GROUP_12,123.123.123.123,1234;P3_GROUP_34,234.234.234.234,4567;");
    // std::string commandstring7("KEEPALIVE,12");
    // std::string commandstring8("LEAVE,12.13.14.15,2345");

    // DEBUG: FEAT: THESE SHOULD NOT WORK ROUND 1
    // std::string commandstring1("SEND_MSG,");
    // std::string commandstring2("LISTSERVERS,");
    // std::string commandstring3("GET_MSG,");
    // std::string commandstring4("STATUSRESP,");
    // std::string commandstring5("STATUSREQ,");
    // std::string commandstring6("SERVERS,");
    // std::string commandstring7("KEEPALIVE,");
    // std::string commandstring8("LEAVE,");

    // DEBUG: FEAT: THESE SHOULD NOT WORK ROUND 2
    std::string commandstring1("SEND_MSG,P3_GROUP_4P3_GROUP_1,The cake is a lie! Do not trust GlaDos. Stay Safe.");
    std::string commandstring2("LISTSERVERS,P_GROUP_12");
    std::string commandstring3("GET_MSG,P3GROUP_234,P3_GROUP_1,");
    std::string commandstring4("STATUSRESP,P3_GROUP_14,P3_GROUP_23,P3_GROUP_4,P3_GROUP_4,");
    std::string commandstring5("STATUSREQ,P3_GROUP_14,P3_GROUP_1,");
    std::string commandstring6("SERVERS,123.123.123.123,1234,P3_GROUP_12;P3_GROUP_34,234.234.234.234,4567;");
    std::string commandstring7("KEEPALIVE,A");
    std::string commandstring8("LEAVE,12.1314.15,2345");


    Command cmd1(commandstring1);
    Command cmd2(commandstring2);
    Command cmd3(commandstring3);
    Command cmd4(commandstring4);
    Command cmd5(commandstring5);
    Command cmd6(commandstring6);
    Command cmd7(commandstring7);
    Command cmd8(commandstring8);

    std::cout << "CMD 1: " << commandstring1 << std::endl;
    std::cout << "CMD 2: " << commandstring2 << std::endl;
    std::cout << "CMD 3: " << commandstring3 << std::endl;
    std::cout << "CMD 4: " << commandstring4 << std::endl;
    std::cout << "CMD 5: " << commandstring5 << std::endl;
    std::cout << "CMD 6: " << commandstring6 << std::endl;
    std::cout << "CMD 7: " << commandstring7 << std::endl;
    std::cout << "CMD 8: " << commandstring8 << std::endl;

    std::cout << "CMD-ID 1: " << cmd1.getID() << std::endl;
    std::cout << "CMD-ID 2: " << cmd2.getID() << std::endl;
    std::cout << "CMD-ID 3: " << cmd3.getID() << std::endl;
    std::cout << "CMD-ID 4: " << cmd4.getID() << std::endl;
    std::cout << "CMD-ID 5: " << cmd5.getID() << std::endl;
    std::cout << "CMD-ID 6: " << cmd6.getID() << std::endl;
    std::cout << "CMD-ID 7: " << cmd7.getID() << std::endl;
    std::cout << "CMD-ID 8: " << cmd8.getID() << std::endl;



    std::cout << std::boolalpha << "CMD-validation 1: " << u.validateCommand(cmd1) << std::endl;
    std::cout << std::boolalpha << "CMD-validation 2: " << u.validateCommand(cmd2) << std::endl;
    std::cout << std::boolalpha << "CMD-validation 3: " << u.validateCommand(cmd3) << std::endl;
    std::cout << std::boolalpha << "CMD-validation 4: " << u.validateCommand(cmd4) << std::endl;
    std::cout << std::boolalpha << "CMD-validation 5: " << u.validateCommand(cmd5) << std::endl;
    std::cout << std::boolalpha << "CMD-validation 6: " << u.validateCommand(cmd6) << std::endl;
    std::cout << std::boolalpha << "CMD-validation 7: " << u.validateCommand(cmd7) << std::endl;
    std::cout << std::boolalpha << "CMD-validation 8: " << u.validateCommand(cmd8) << std::endl;

// PART: SIMULATE MESSAGE/COMMAND STRING PART 2!!

    // std::string commandstring1("SEND_MSG,V_GROUP_4,V_GROUP_1,The cake is a lie! Do not trust GlaDos. Stay Safe.");
    // std::string commandstring2("SEND_MSG,V_GROUP_12,V_GROUP_13,The cake is a lie! Do not trust GlaDos. Stay Safe.");
    // std::string commandstring3("SEND_MSG,V_GROUP_234,V_GROUP_43,The cake is a lie! Do not trust GlaDos. Stay Safe.");
    // std::string commandstring4("SEND_MSG,V_GROUP_14,V_GROUP_23,The cake is a lie! Do not trust GlaDos. Stay Safe.");

    // Command cmd1(commandstring1);
    // Command cmd2(commandstring2);
    // Command cmd3(commandstring3);
    // Command cmd4(commandstring4);

    // Message msg1(cmd1);
    // Message msg2(cmd2);
    // Message msg3(cmd3);
    // Message msg4(cmd4);

    // std::cout << "##################################################" << std::endl;
    // std::cout << "Command ID: " << cmd1.getID() << std::endl;
    // std::cout << "payload length: " << cmd1.getPayload().size() << std::endl;
    // for(auto i: cmd1.getPayload()) std::cout << i << std::endl;
    // std::cout << "##################################################" << std::endl;
    // std::cout << "Command ID: " << cmd2.getID() << std::endl;
    // std::cout << "payload length: " << cmd2.getPayload().size() << std::endl;
    // for(auto i: cmd2.getPayload()) std::cout << i << std::endl;
    // std::cout << "##################################################" << std::endl;
    // std::cout << "Command ID: " << cmd3.getID() << std::endl;
    // std::cout << "payload length: " << cmd3.getPayload().size() << std::endl;
    // for(auto i: cmd3.getPayload()) std::cout << i << std::endl;
    // std::cout << "##################################################" << std::endl;
    // std::cout << "Command ID: " << cmd4.getID() << std::endl;
    // std::cout << "payload length: " << cmd4.getPayload().size() << std::endl;
    // for(auto i: cmd4.getPayload()) std::cout << i << std::endl;
    // std::cout << "##################################################" << std::endl;

    // std::cout << "Logging..." << std::endl;
    // msg1.logMessage(cmd1.getID());
    // msg2.logMessage(cmd2.getID());
    // msg3.logMessage(cmd3.getID());
    // msg4.logMessage(cmd4.getID());
    // std::cout << "Logging... Finished!" << std::endl;

    // std::cout << "##################################################" << std::endl;

    return 0;
}






// // SECTION: STRING => RAW BYTE => STRING
    // std::string input("<my command>");

    // std::cout << "###\nPlain string: " << input << std::endl;
    // std::cout << "Plain string length: " << input.size() << std::endl;

    //std::vector<std::byte> rawVec{getRawBytes(input)};
    // std::cout << "###\nByte version of string with SOH/EOT: ";
    // auto rawVec{getRawBytes(input)};
    // for(auto i: rawVec) std::cout << std::hex << (size_t)i << " ";
    // std::cout << std::endl;
    // std::cout << "Length byte vector: " << std::dec << rawVec.size() << " bytes" << std::endl;

    // std::string rebuiltString;
    // for(auto byte: rawVec) rebuiltString.push_back((char)byte);
    // std::cout << "###\nString rebuilt from byte vector: " << rebuiltString << std::endl;
    // std::cout << "String length: " << rebuiltString.size() << std::endl;


// // SECTION: REGEX KNOWN COMMANDS

    // // COM: List of potential commands.
    // std::array<std::string,10> commands{"LEAVE","listservers","keepaLIVE","GET MSG","Servers","send msg","DELETE","statusreq","statusresp","statusrespt"};



// PART: ToLower TRANSFORM
    // std::cout << "\n######### Initial: " << std::endl;
    // for(auto i: commands) std::cout << i << std::endl;

    // // COM: ToLower() / ToUpper()
    // for(auto &i: commands) std::transform(i.begin(), i.end(), i.begin(), [] (unsigned char c){ return std::toupper(c); }); // std::toupper(c)tolower

    // std::cout << "\n######### Transformed: " << std::endl;
    // for(auto i: commands) std::cout << i << std::endl;

// PART: FILTER FOR SOH & EOT.
    // std::regex soh_eot("[\x01]|[\x04]");

    // std::cout << "\n######### RegExing SOH/EOT" << std::endl;

    // // Check if ready for valdation [match.ready()]

    // for(size_t i = 0; i < rawVec.size(); i++){
    //     std::smatch match;
    //     std::string temp{(char)rawVec[i]};
    //     std::regex_search(temp, match, soh_eot);
    //     std::cout << std::boolalpha;
    //     std::cout << "Matches found: " << !match.empty() << std::endl;
    //     std::cout << "Number of matches: " << match.size() << std::endl;
    //     std::cout << std::endl;
    // }

// PART: FILTER FOR KNOWN COMMANDS
    // std::cout << "\n######### RegExing yo" << std::endl;

    // std::regex knownCommands("leave|(list)*servers$|keepalive|[\\s]msg$|statusre(q$|sp$)");

    // for(size_t i = 0; i < commands.size(); i++){
    //     std::smatch match;
    //     std::regex_search(commands[i], match, knownCommands);
    //     std::cout << std::boolalpha;
    //     std::cout << "Matches found: " << !match.empty() << std::endl;
    //     std::cout << "Number of matches: " << match.size() << std::endl;
    //     if(!match.empty()) std::cout << "The match: " << match.str() << "\nFor word: " << commands[i] << std::endl;
    //     else std::cout << "No match: " << commands[i] << std::endl;
    //     std::cout << std::endl;
    // }

// // PART: EXPERIMENT WITH TIME
    // const auto timeStamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // std::cout << "Number of seconds since Epoch: " << (size_t)timeStamp << std::endl;

// PART: SIMULATE MESSAGE/COMMAND STRING

    // std::string str1("SERVERS,V_GROUP_1,130.208.243.61,8888;V_GROUP_2,10.2.132.12,888;");
    // std::cout << "###\nPlain string: " << str << std::endl;
    // auto bufferPayload{getRawBytes(str)};
    // std::cout << "###\nByte version of bufferPayload with SOH/EOT:\n";
    // for(size_t i{1}; i <= bufferPayload.size(); ++i) {
    //     std::cout << std::hex << (size_t)bufferPayload.at(i-1) <<((i % 16 == 0) ? "\n" : " ");
    // }


    // std::string v{"01234;5678 AeGiR er ad fara heim;"};
    // std::string x{"01234 5678 AeGiR er ad fara heim"};

    // if(std::any_of(std::begin(v), std::end(v), [](char c) { return c == ';'; })) {
    //     std::cout << "Yes it has a \";\"" << std::endl;
    // }else{
    //     std::cout << "No it does not have a \";\"" << std::endl;
    // }

    // if(std::any_of(std::begin(x), std::end(x), [](char c) { return c == ';'; })) {
    //     std::cout << "Yes it has a \";\"" << std::endl;
    // }else{
    //     std::cout << "No it does not have a \";\"" << std::endl;
    // }

    // std::vector<std::string> asdf;

    // std::cout << asdf.max_size() << std::endl;
