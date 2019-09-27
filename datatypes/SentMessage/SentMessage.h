#ifndef SENTMESSAGE_H
#define SENTMESSAGE_H

#include <ctime>
#include <chrono>
#include <cstring>
#include <ostream>

class SentMessage {
    public:
        SentMessage();
        SentMessage(std::string recipient, std::string message, size_t timeStamp);
        virtual ~SentMessage();

        friend std::ostream& operator << (std::ostream& outs, const SentMessage& sentMessage);

    private:
        char recipient[12];
        char message[5000];
        size_t timeStamp;
};

#endif // SENTMESSAGE_H

