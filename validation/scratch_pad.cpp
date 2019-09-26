#include <iostream>
#include <regex>
// #include <bitset>
#include <vector>
#include <array>
// #include <string>
#include <cctype>

std::vector<std::byte> getRawBytes(std::string str);

int main(int argc, char *argv[]){

    // PART: STRING => RAW BYTE
    std::string input("<my command>");

    std::cout << "Plain string: " << input << std::endl;

    // std::vector<std::byte> rawVec{getRawBytes(input)};
    auto rawVec{getRawBytes(input)};

    std::cout << "Byte version with SOH/EOT: ";
    for(auto i: rawVec) std::cout << std::hex << (size_t)i << " ";
    std::cout << std::endl;
    std::cout << "Payload size: " << std::dec << rawVec.size() << " bytes." << std::endl;

    // PART: REGEX KNOWN COMMANDS

    std::array<std::string,10> commands{"LEAVE","listservers","keepaLIVE","GET MSG","Servers","send msg","DELETE","statusreq","statusresp","statusrespt"};



    std::cout << "\n######### Initial: " << std::endl;
    for(auto i: commands) std::cout << i << std::endl;

    // ToLower()
    for(auto &i: commands){
        std::transform(i.begin(), i.end(), i.begin(), [] (unsigned char c){ return std::tolower(c); });
    }

    std::cout << "\n######### Transformed: " << std::endl;
    for(auto i: commands) std::cout << i << std::endl;

    // RegEx fishing ############



    // std::regex soh("[\x01]");
    // std::regex eot("[\x04]");
    std::regex soh_eot("[\x01]|[\x04]");

    std::cout << "\n######### RegExing SOH/EOT" << std::endl;

    for(size_t i = 0; i < rawVec.size(); i++){

        std::smatch match;
        std::string temp{(char)rawVec[i]};
        std::regex_search(temp, match, soh_eot);

        std::cout << std::boolalpha;
        // std::cout << "Checked for Results: " << match.ready() << std::endl;
        std::cout << "Matches found: " << !match.empty() << std::endl;
        std::cout << "Number of matches: " << match.size() << std::endl;

        std::cout << std::endl;
    }

    std::cout << "\n######### RegExing yo" << std::endl;

    std::regex knownCommands("leave|(list)*servers$|keepalive|[\\s]msg$|statusre(q$|sp$)");

    for(size_t i = 0; i < commands.size(); i++){

        std::smatch match;
        std::regex_search(commands[i], match, knownCommands);

        std::cout << std::boolalpha;
        // std::cout << "Checked for Results: " << match.ready() << std::endl;
        std::cout << "Matches found: " << !match.empty() << std::endl;
        std::cout << "Number of matches: " << match.size() << std::endl;

        if(!match.empty()) std::cout << "The match: " << match.str() << "\nFor word: " << commands[i] << std::endl;
        else std::cout << "No match: " << commands[i] << std::endl;

        std::cout << std::endl;
    }
    return 0;
}

// char asciitolower(char in){
//     if (in <= 'Z' && in >= 'A')
//         return in - ('Z' - 'z');
//     return in;
// }
// std::transform(data.begin(), data.end(), data.begin(), asciitolower);

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

// Aexx