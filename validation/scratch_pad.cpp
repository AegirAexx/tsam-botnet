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


int main(int argc, char *argv[]){

// PART: SIMULATE MESSAGE/COMMAND STRING PART 2!!

    std::string commandstring1("SEND_MSG,V_GROUP_4,V_GROUP_1,The cake is a lie! Do not trust GlaDos. Stay Safe.");
    std::string commandstring2("SEND_MSG,V_GROUP_12,V_GROUP_13,The cake is a lie! Do not trust GlaDos. Stay Safe.");
    std::string commandstring3("SEND_MSG,V_GROUP_234,V_GROUP_43,The cake is a lie! Do not trust GlaDos. Stay Safe.");
    std::string commandstring4("SEND_MSG,V_GROUP_14,V_GROUP_23,The cake is a lie! Do not trust GlaDos. Stay Safe.");

    Command cmd1(commandstring1);
    Command cmd2(commandstring2);
    Command cmd3(commandstring3);
    Command cmd4(commandstring4);

    Message msg1(cmd1);
    Message msg2(cmd2);
    Message msg3(cmd3);
    Message msg4(cmd4);

    std::cout << "##################################################" << std::endl;
    std::cout << "Command ID: " << cmd1.getID() << std::endl;
    std::cout << "payload length: " << cmd1.getPayload().size() << std::endl;
    for(auto i: cmd1.getPayload()) std::cout << i << std::endl;
    std::cout << "##################################################" << std::endl;
    std::cout << "Command ID: " << cmd2.getID() << std::endl;
    std::cout << "payload length: " << cmd2.getPayload().size() << std::endl;
    for(auto i: cmd2.getPayload()) std::cout << i << std::endl;
    std::cout << "##################################################" << std::endl;
    std::cout << "Command ID: " << cmd3.getID() << std::endl;
    std::cout << "payload length: " << cmd3.getPayload().size() << std::endl;
    for(auto i: cmd3.getPayload()) std::cout << i << std::endl;
    std::cout << "##################################################" << std::endl;
    std::cout << "Command ID: " << cmd4.getID() << std::endl;
    std::cout << "payload length: " << cmd4.getPayload().size() << std::endl;
    for(auto i: cmd4.getPayload()) std::cout << i << std::endl;
    std::cout << "##################################################" << std::endl;
    std::cout << "Logging..." << std::endl;

    msg1.logMessage();
    msg2.logMessage();
    msg3.logMessage();
    msg4.logMessage();


    std::cout << "Logging... Finished!" << std::endl;
    std::cout << "##################################################" << std::endl;


    return 0;
}

// PART: COMMANDS - MINIMUM REQUIRED
    // LISTSERVERS,<FROM_GROUP_ID>
    // KEEPALIVE,<# of Messages>
    // GET MSG,<GROUP_ID>
    // SEND MSG,<FROM_GROUP_ID>,<TO_GROUP_ID>,<Message content>
    // LEAVE,SERVER_IP,PORT
    // STATUSREQ,FROM_GROUP
    // CONNECT ??
// PART:



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
