
#include "Message.h"


Message::Message(Command cmd) {
    this->from = cmd.getPayload()[0];
    this->to = cmd.getPayload()[1];
    for (size_t i = 2; i < cmd.getPayload().size(); i++) this->msg += cmd.getPayload()[i] + " ";
    this->msg.pop_back();
    this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


Message::Message(std::string from, std::string to, std::string msg) {
    this->from = from;
    this->to = to;
    this->msg = msg;
    this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


Message::Message(std::string from, std::string to, std::string msg, size_t timeStamp) {
    this->from = from;
    this->to = to;
    this->msg = msg;
    this->timeStamp = timeStamp;
}


Message::~Message() {}


std::string Message::getFormattedMessage() {
    return this->from + "," + this->to + "," + this->msg;
}

std::string Message::getFrom() {
    return this->from;
}


std::string Message::getTo() {
    return this->to;
}


std::string Message::getMsg() {
    return this->msg;
}


size_t Message::getTimeStamp() {
    return this->timeStamp;
}


void Message::logMessage() {
    std::ofstream outStream;
    outStream.open("logs/MessageLog.txt", std::ios::app);
    if(outStream.is_open()) outStream << *this;
    outStream.close();
}

std::ostream& operator << (std::ostream& outs, const Message& msg) {
    Message tempMessage = msg;
    std::time_t t = static_cast<std::time_t>(tempMessage.timeStamp);
    outs << "FROM: " << tempMessage.from
         << " | TO: " << tempMessage.to
         << " | MSG: " << tempMessage.msg
         << " | TIMESTAMP: " << std::put_time(std::gmtime(&t), "%d.%m.%y -=- %H:%M:%S")
    << std::endl;
    return outs;
}

// #include "Message.h"

// Message::Message(std::string from, std::string to, std::string msg) {
//     this->from = from;
//     this->to = to;
//     this->msg = msg;
//     this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
// }

// Message::Message(Command cmd) {
//     this->from = cmd.getPayload()[0];
//     this->to = cmd.getPayload()[1];
//     for (size_t i = 2; i < cmd.getPayload().size(); i++) this->msg += cmd.getPayload()[i] + " ";
//     this->msg.pop_back();
//     this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
// }

// Message::~Message() {}

// std::ostream& operator << (std::ostream& outs, const Message& msg) {
//     Message tempMessage = msg;
//     std::time_t t = static_cast<std::time_t>(tempMessage.timeStamp);
//     outs << "--\nFROM: " << tempMessage.from
//          << "\n--\nTO: " << tempMessage.to
//          << "\n--\nPAYLOAD:\n" << tempMessage.msg
//          << "\n--\nTIMESTAMP: " << std::put_time(std::gmtime(&t), "%d.%m.%y|%H:%M:%S"  )
//     << std::endl;
//     return outs;
// }

// std::string Message::getFrom() {
//     return this->from;
// }

// std::string Message::getTo() {
//     return this->to;
// }

// std::string Message::getMsg() {
//     return this->msg;
// }

// size_t Message::getTimeStamp() {
//     return this->timeStamp;
// }
