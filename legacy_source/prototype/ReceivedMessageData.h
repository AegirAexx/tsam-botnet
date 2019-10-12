#ifndef RECEIVEDMESSAGEDATA_H
#define RECEIVEDMESSAGEDATA_H

#include <fstream>
#include <vector>
#include "../model/ReceivedMessage.h"


class ReceivedMessageData{
    public:
        ReceivedMessageData();
        std::vector<ReceivedMessage> getReceivedMessages();
        void write(ReceivedMessage server);
        bool checkFile(bool x);

};

#endif // RECEIVEDMESSAGEDATA_H
