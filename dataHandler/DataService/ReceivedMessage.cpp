// TODO: VALIDATION!!

#include "ReceivedMessage.h"

ReceivedMessage::ReceivedMessage(){
    strcpy (this->recipient, "NOID_NOID");
    strcpy (this->sender, "NOID_NOID");
    memset (this->message, 0, 5000);
    // this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    this->timeStamp = 0;
}

ReceivedMessage::ReceivedMessage(std::string recipient, std::string sender, std::string message, size_t timeStamp){
    strcpy (this->recipient, recipient.c_str());
    strcpy (this->recipient, sender.c_str());
    memset (this->message, 0, 5000);
    strcpy (this->message, message.c_str());
    this->timeStamp = timeStamp;
}

ReceivedMessage::~ReceivedMessage() {}

// Add a pretty print overload
std::ostream& operator << (std::ostream& outs, const ReceivedMessage& message){
    ReceivedMessage tempMessage = message; // Temporary variable to circumvent "const" restrictions.
    std::time_t t = static_cast<std::time_t>(tempMessage.timeStamp);
    outs << "--\nTO:   " << tempMessage.recipient
         << "\nFROM: " << tempMessage.recipient
         << "\n--\nPAYLOAD:\n" << tempMessage.message
         << "\n--\nTIMESTAMP: " << std::put_time(std::gmtime(&t), "%d.%m.%y|%H:%M:%S"  )
    << std::endl;
    return outs;
}