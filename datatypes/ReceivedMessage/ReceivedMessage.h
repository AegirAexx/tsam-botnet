#ifndef RECEIVEDMESSAGE_H
#define RECEIVEDMESSAGE_H

#include <ctime>
#include <chrono>
#include <cstring>
#include <ostream>

class ReceivedMessage {
    public:
        ReceivedMessage();
        ReceivedMessage(std::string recipient, std::string sender, std::string message, size_t timeStamp);
        virtual ~ReceivedMessage();

        friend std::ostream& operator << (std::ostream& outs, const ReceivedMessage& receivedMessage);

    private:
        char recipient[12];
        char sender[12];
        char message[5000];
        size_t timeStamp;
};

#endif // RECEIVEDMESSAGE_H

