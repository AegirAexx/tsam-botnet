
#include "../../include/data/SentMessageData.h"

SentMessageData::SentMessageData(){}

std::vector<SentMessage> SentMessageData::getSentMessages(){

    std::vector<SentMessage> sentMessages;
    std::ifstream input;
    input.open("data/SentMessageData.dat", std::ios::binary);

    input.seekg(0, input.end);
    size_t totalLogged {input.tellg() / sizeof(SentMessage)};
    input.seekg(0, input.beg);

    SentMessage *dataPointer = new SentMessage[totalLogged];
    input.read((char*)dataPointer, sizeof(SentMessage) * totalLogged);

    input.close();

    for(size_t i{0}; i < totalLogged; ++i){
        sentMessages.push_back(dataPointer[i]);
    }

    // TODO: MEMORY LEAKS??
    // delete [] dataPointer; // BUG:

    return sentMessages;
}

void SentMessageData::write(SentMessage server){

    std::ofstream output;
    output.open("data/ServerProfileData", std::ios::binary|std::ios::app);
    output.write((char*)(&server), sizeof(SentMessage));
    output.close();

}

bool SentMessageData::checkFile(bool x){
    return !x;
}