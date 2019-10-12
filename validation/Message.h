#ifndef MESSAGE_H
#define MESSAGE_H

#include <ctime>
#include <chrono>
#include <ostream>
#include <iomanip>
#include "Command.h"

class Message {
    public:
        Message(std::string from, std::string to, std::string msg);
        Message(Command cmd);
        virtual ~Message();

        std::string getFrom();
        std::string getTo();
        std::string getMsg();
        size_t getTimeStamp();
        friend std::ostream& operator << (std::ostream& outs, const Message& msg);

    private:
        std::string from;
        std::string to;
        std::string msg;
        size_t timeStamp;
};

#endif // MESSAGE_H