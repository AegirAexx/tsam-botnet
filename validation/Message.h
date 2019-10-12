#ifndef MESSAGE_H
#define MESSAGE_H

#include <ctime>
#include <chrono>
#include <ostream>
#include <fstream>
#include <iomanip>
#include "Command.h"

class Message {
    public:
        Message(Command cmd);
        Message(std::string from, std::string to, std::string msg);
        Message(std::string from, std::string to, std::string msg, size_t timeStamp);
        virtual ~Message();

        std::string getFrom();
        std::string getTo();
        std::string getMsg();
        size_t getTimeStamp();
        std::string getFormattedMessage();

        void logMessage();

        friend std::ostream& operator << (std::ostream& outs, const Message& msg);
        friend bool operator < (const Message& rhs, const Message& lhs);

    private:
        std::string from;
        std::string to;
        std::string msg;
        size_t timeStamp;
};

#endif // MESSAGE_H