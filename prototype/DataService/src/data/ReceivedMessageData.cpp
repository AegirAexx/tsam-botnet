
#include "../../include/data/ReceivedMessageData.h"

ReceivedMessageData::ReceivedMessageData(){}

std::vector<ReceivedMessage> ReceivedMessageData::getReceivedMessages(){

    std::vector<ReceivedMessage> receivedMessags;
    std::ifstream input;
    input.open("data/ReceivedMessageData.dat", std::ios::binary);

    input.seekg(0, input.end);
    size_t totalLogged {input.tellg() / sizeof(ReceivedMessage)};
    input.seekg(0, input.beg);

    ReceivedMessage *dataPointer = new ReceivedMessage[totalLogged];
    input.read((char*)dataPointer, sizeof(ReceivedMessage) * totalLogged);

    input.close();

    for(size_t i{0}; i < totalLogged; ++i){
        receivedMessags.push_back(dataPointer[i]);
    }

    // TODO: MEMORY LEAKS??
    // delete [] dataPointer; // BUG:

    return receivedMessags;
}

void ReceivedMessageData::write(ReceivedMessage server){

    std::ofstream output;
    output.open("data/ServerProfileData", std::ios::binary|std::ios::app);
    output.write((char*)(&server), sizeof(ReceivedMessage));
    output.close();

}

bool ReceivedMessageData::checkFile(bool x){
    return !x;
}