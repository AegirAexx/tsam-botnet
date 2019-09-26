#include <iostream>
#include <regex>
// #include <bitset>
#include <vector>
#include <array>
// #include <string>
#include <cctype>

std::vector<std::byte> getRawBytes(std::string str);

int main(int argc, char *argv[]){

    // // SECTION: STRING => RAW BYTE => STRING
    std::string input("<my command>");

    std::cout << "###\nPlain string: " << input << std::endl;
    std::cout << "Plain string length: " << input.size() << std::endl;

    // std::vector<std::byte> rawVec{getRawBytes(input)};
    std::cout << "###\nByte version of string with SOH/EOT: ";
    auto rawVec{getRawBytes(input)};
    for(auto i: rawVec) std::cout << std::hex << (size_t)i << " ";
    std::cout << std::endl;
    std::cout << "Length byte vector: " << std::dec << rawVec.size() << " bytes" << std::endl;

    std::string rebuiltString;
    for(auto byte: rawVec) rebuiltString.push_back((char)byte);
    std::cout << "###\nString rebuilt from byte vector: " << rebuiltString << std::endl;
    std::cout << "String length: " << rebuiltString.size() << std::endl;


    // SECTION: REGEX KNOWN COMMANDS

    // COM: List of potential commands.
    std::array<std::string,10> commands{"LEAVE","listservers","keepaLIVE","GET MSG","Servers","send msg","DELETE","statusreq","statusresp","statusrespt"};



    // PART: ToLower TRANSFORM
    std::cout << "\n######### Initial: " << std::endl;
    for(auto i: commands) std::cout << i << std::endl;

    // COM: ToLower()
    for(auto &i: commands) std::transform(i.begin(), i.end(), i.begin(), [] (unsigned char c){ return std::tolower(c); });

    std::cout << "\n######### Transformed: " << std::endl;
    for(auto i: commands) std::cout << i << std::endl;

    // PART: FILTER FOR SOH & EOT.
    std::regex soh_eot("[\x01]|[\x04]");

    std::cout << "\n######### RegExing SOH/EOT" << std::endl;

    // Check if ready for valdation [match.ready()]

    for(size_t i = 0; i < rawVec.size(); i++){
        std::smatch match;
        std::string temp{(char)rawVec[i]};
        std::regex_search(temp, match, soh_eot);
        std::cout << std::boolalpha;
        std::cout << "Matches found: " << !match.empty() << std::endl;
        std::cout << "Number of matches: " << match.size() << std::endl;
        std::cout << std::endl;
    }

    // PART: FILTER FOR KNOWN COMMANDS
    std::cout << "\n######### RegExing yo" << std::endl;

    std::regex knownCommands("leave|(list)*servers$|keepalive|[\\s]msg$|statusre(q$|sp$)");

    for(size_t i = 0; i < commands.size(); i++){
        std::smatch match;
        std::regex_search(commands[i], match, knownCommands);
        std::cout << std::boolalpha;
        std::cout << "Matches found: " << !match.empty() << std::endl;
        std::cout << "Number of matches: " << match.size() << std::endl;
        if(!match.empty()) std::cout << "The match: " << match.str() << "\nFor word: " << commands[i] << std::endl;
        else std::cout << "No match: " << commands[i] << std::endl;
        std::cout << std::endl;
    }
    return 0;
}

// This function stuffs 0x01 and 0x04 on either end.
// could be used to construct outgoing messages.
std::vector<std::byte> getRawBytes(std::string str){
    std::vector<std::byte> rawBytes;
    rawBytes.push_back((std::byte)0x01);
    for(auto charByte: str) rawBytes.push_back((std::byte)charByte);
    rawBytes.push_back((std::byte)0x04);
    return rawBytes;
}

// PART: COMMANDS - MINIMUM REQUIRED
    // LISTSERVERS,<FROM_GROUP_ID>
    // SERVERS
    // SERVERS,V_GROUP_1,130.208.243.61,8888;V_GROUP_2,10.2.132.12,888;
    // KEEPALIVE,<# of Messages>
    // GET MSG,<GROUP_ID>
    // SEND MSG,<FROM_GROUP_ID>,<TO_GROUP_ID>,<Message content>
    // LEAVE,SERVER_IP,PORT
    // STATUSREQ,FROM_GROUP
    // STATUSRESP,FROM_GROUP,TO_GROUP,<server, msgs held>,...
    // STATUSRESP,V_GROUP_2,I_1,V_GROUP4,20,V_GROUP71,2
// PART:
