// TODO: VALIDATION!!

#include "SentMessage.h"

SentMessage::SentMessage(){
    strcpy (this->recipient, "NOID_NOID");
    memset (this->message, 0, 5000);
    this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

SentMessage::SentMessage(std::string recipient, std::string message, size_t timeStamp){
    strcpy (this->recipient, recipient.c_str());
    memset (this->message, 0, 5000);
    strcpy (this->message, message.c_str());
    this->timeStamp = timeStamp;
}

SentMessage::~SentMessage() {}