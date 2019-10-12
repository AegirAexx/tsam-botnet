#ifndef MESSAGE_H
#define MESSAGE_H

#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Command.h"

class Message {
    public:
        Message();
        Message(Command cmd);
        Message(std::string from, std::string to, std::string msg);
        Message(std::string from, std::string to, std::string msg, size_t timeStamp);
        virtual ~Message();

        std::string getFrom();
        std::string getTo();
        std::string getMsg();
        size_t getTimeStamp();
        std::string getFormattedMessage();

        std::vector<std::string> getMessages();
        void logMessage();
        std::vector<std::string> split(std::string stringToSplit, char delimeter);

        friend std::ostream& operator << (std::ostream& outs, const Message& msg);
        friend bool operator < (const Message& rhs, const Message& lhs);

    private:
        std::string from;
        std::string to;
        std::string msg;
        size_t timeStamp;
};

#endif // MESSAGE_H