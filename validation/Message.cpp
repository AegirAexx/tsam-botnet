
#include "Message.h"

Message::Message() {
    this->from = nullptr;
    this->to = nullptr;
    this->msg = nullptr;
    this->timeStamp = 0;
}

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


std::vector<std::string> Message::getMessages() {
    std::vector<Message> messages;
    std::vector<std::string> buffers;
    std::vector<std::string> tokens;
    std::string buffer;
    std::ifstream inStream;
    inStream.open("logs/MessageLog.txt", std::ios::in);
    if(inStream.is_open()) {
        while(!inStream.eof()) {
            getline(inStream, buffer);
            buffers.push_back(buffer);
        }
        inStream.close();
    }
    return buffers;
    // for(size_t i = 0; i < buffers.size(); ++i) {
    //     tokens = split(buffers[i], ',');
    //     Message msg(tokens[0],tokens[1],tokens[2],(size_t)atoi(tokens[3].c_str()));
    //     messages.push_back(msg);
    //     tokens.clear();
    // }
    // return messages;
}


void Message::logMessage() {
    std::ofstream outStream;
    outStream.open("logs/MessageLog.txt", std::ios::app);
    if(outStream.is_open()) outStream << *this;
    outStream.close();
}


std::vector<std::string> Message::split(std::string stringToSplit, char delimeter) {
    std::stringstream ss(stringToSplit);
    std::string word;
	std::vector<std::string> splittedStrings;
    while (std::getline(ss, word, delimeter)) splittedStrings.push_back(word);
    return splittedStrings;
}


std::ostream& operator << (std::ostream& outs, const Message& msg) {
    Message tempMessage = msg;
    std::time_t t = static_cast<std::time_t>(tempMessage.timeStamp);
    outs << tempMessage.from
         << "," << tempMessage.to
         << "," << tempMessage.msg
         << "," << tempMessage.timeStamp
         << "," << std::put_time(std::gmtime(&t), "%d.%m.%y-%H:%M:%S"  )
    << std::endl;
    return outs;
}
