#ifndef LOGDATA_H
#define LOGDATA_H

#include <iostream>
#include <string>
#include <ctime>

class LogData {
    public:
        LogData();
        LogData(int id, std::string message, std::time_t timeStamp);
        virtual ~LogData() {}

        friend std::ostream& operator << (std::ostream& outs, const LogData& logData);

        void addTimeStamp();
        void setMessage(std::string message);
        std::string getMessage();

    private:
        int id;
        std::string message;
        std::time_t timeStamp;
};

#endif // LOGDATA_H

// TODO: DESIGN THE DATA TYPE AS NEEDED
// FEAT: IP number, Misc Header DATA...
