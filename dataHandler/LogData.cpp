#include "LogData.h"

LogData::LogData():
    id(0),
    message("SERVERS,V_GROUP_1,130.208.243.61,8888"),
    timeStamp(std::time(nullptr))
{}

LogData::LogData(int id, std::string message, std::time_t timeStamp) :
    id(id),
    message(message),
    timeStamp(std::time(nullptr))
{}

// virtual ~LogData() {}


friend std::ostream& operator << (std::ostream& outs, const LogData& logData);

std::ostream& operator << (std::ostream& outs, const LogData& logData){
    // Time tempNorm = time; // Temporary variable to circumvent "const" restrictions.
    // tempNorm.normalize(); // Output normalized before being printed.
    // outs << std::setfill ('0') << std::setw (2) << tempNorm.hours << ":"
    //      << std::setfill ('0') << std::setw (2) << tempNorm.minutes << ":"
    //      << std::setfill ('0') << std::setw (2) << tempNorm.seconds << std::endl;
    return outs;
}

void LogData::addTimeStamp(){

}
void LogData::setMessage(std::string message){

}
std::string LogData::getMessage(){
    return "typpi";
}
