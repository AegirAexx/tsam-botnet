// COM: T-409-TSAM-2019-3 | Project 3 - The Botnet Rises
// COM: Aegir_Tomasson[aegir15] && Dagur_Kristjansson[dagur17]

#ifndef MESSAGE_H
#define MESSAGE_H

#include "Command.h"

#include <ctime>
#include <chrono>
#include <ostream>
#include <fstream>
#include <iomanip>

// COM: This is our datatype to hold messages and log to file.

class Message {
    public:
    // COM: Class constructor / Destructor.
        Message(std::string from, std::string to, std::string msg);
        virtual ~Message();

    // COM: Class getters.
        std::string getFrom();
        std::string getTo();
        std::string getMsg();
        size_t getTimeStamp();
        bool getIsSend();
        bool getIsClient();
        std::string getFormattedMessage();
        int getGroupID();

    // COM: Class data access.
        void logMessage(int id);

    // COM: Formatting for ostream.
        friend std::ostream& operator << (std::ostream& outs, const Message& msg);

    private:
    // Class variables.
        std::string from;
        std::string to;
        std::string msg;
        int groupID;
        size_t timeStamp;
        bool isClient;
        bool isSend;
    // Class utility
        std::vector<std::string> split(std::string stringToSplit, char delimeter);

};

#endif // MESSAGE_H
