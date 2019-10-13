
#include "Message.h"

// if(command == "SEND_MSG") this->id = 1;
// else if(command == "GET_MSG") this->id = 2;
// else if(command == "KEEPALIVE") this->id = 3;
// else if(command == "CONNECT") this->id = 4;
// else if(command == "LEAVE") this->id = 5;
// else if(command == "STATUSREQ") this->id = 6;
// else if(command == "LISTSERVERS") this->id = 7;
// else if(command == "SERVERS") this->id = 8;
// else if(command == "STATUSRESP") this->id = 9;
// else if(command == "GETMSG") this->id = 10;
// else if(command == "SENDMSG") this->id = 11;

Message::Message(Command cmd) {
    if(cmd.getID() == 1 || cmd.getID() == 11) this->isSend = true;
    else this->isSend = false;
    if(cmd.getID() == 10 || cmd.getID() == 11) this->isClient = true;
    else this->isClient = false;
    if(cmd.getID() == 11) {
        this->from = "P3_GROUP_4";
        this->to = cmd.getPayload()[0];
        for (size_t i = 1; i < cmd.getPayload().size(); i++) this->msg += cmd.getPayload()[i] + " ";

    } else {
        this->from = cmd.getPayload()[0];
        this->to = cmd.getPayload()[1];
        for (size_t i = 2; i < cmd.getPayload().size(); i++) this->msg += cmd.getPayload()[i] + " ";
    }
    this->msg.pop_back();
    this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    this->groupID = getGroupID();
}


// Message::Message(std::string from, std::string to, std::string msg) {
//     this->from = from;
//     this->to = to;
//     this->msg = msg;
//     this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//     this->groupID = getGroupID();
// }


// Message::Message(std::string from, std::string to, std::string msg, size_t timeStamp) {
//     this->from = from;
//     this->to = to;
//     this->msg = msg;
//     this->timeStamp = timeStamp;
// }


int Message::getGroupID() {
    auto temp = split(this->to, '_');
    return atoi(temp[temp.size()-1].c_str());
}

std::vector<std::string> Message::split(std::string stringToSplit, char delimeter) {
    std::stringstream ss(stringToSplit);
    std::string word;
	std::vector<std::string> splittedStrings;
    while (std::getline(ss, word, delimeter)) splittedStrings.push_back(word);
    return splittedStrings;
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


bool Message::getIsSend() {
    return this->isSend;
}


bool Message::getIsClient() {
    return this->isClient;
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
    outs << "COMMAND: " <<  (tempMessage.isSend ? "SEND" : "GET")
         << " | isBot: " << (!tempMessage.isClient ? "TRUE" : "FALSE")
         << " | FROM: " << tempMessage.from
         << " | TO: " << tempMessage.to
         << " | MSG: " << tempMessage.msg
         << " | TIMESTAMP: " << std::put_time(std::gmtime(&t), "%d.%m.%y -=- %H:%M:%S")
    << std::endl;
    return outs;
}

