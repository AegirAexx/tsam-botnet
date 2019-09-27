// TODO: VALIDATION!!

#include "ReceivedMessage.h"

ReceivedMessage::ReceivedMessage(){
    strcpy (this->recipient, "NOID_NOID");
    strcpy (this->sender, "NOID_NOID");
    memset (this->message, 0, 5000);
    this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

ReceivedMessage::ReceivedMessage(std::string recipient, std::string sender, std::string message, size_t timeStamp){
    strcpy (this->recipient, recipient.c_str());
    strcpy (this->recipient, sender.c_str());
    memset (this->message, 0, 5000);
    strcpy (this->message, message.c_str());
    this->timeStamp = timeStamp;
}

ReceivedMessage::~ReceivedMessage() {}