#ifndef SENTMESSAGEDATA_H
#define SENTMESSAGEDATA_H

#include <fstream>
#include <vector>
#include "../model/SentMessage.h"


class SentMessageData{
    public:
        SentMessageData();
        std::vector<SentMessage> getSentMessages();
        void write(SentMessage server);
        bool checkFile(bool x);

};

#endif // SENTMESSAGEDATA_H
