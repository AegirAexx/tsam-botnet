
#include "Message.h"

Message::Message(std::string from, std::string to, std::string msg) {
    this->from = from;
    this->to = to;
    this->msg = msg;
    this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Message::Message(Command cmd) {
    this->from = cmd.getPayload()[0];
    this->to = cmd.getPayload()[1];
    for (size_t i = 2; i < cmd.getPayload().size(); i++) this->msg += cmd.getPayload()[i] + " ";
    this->msg.pop_back();
    this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Message::~Message() {}

std::ostream& operator << (std::ostream& outs, const Message& msg) {
    Message tempMessage = msg;
    std::time_t t = static_cast<std::time_t>(tempMessage.timeStamp);
    outs << "[FROM: " << tempMessage.from
         << " | TO: " << tempMessage.to
         << " | TIMESTAMP: " << std::put_time(std::gmtime(&t), "%d.%m.%y - %H:%M:%S"  )
         << " | MSG: " << tempMessage.msg
         << "]"
    << std::endl;
    return outs;
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
