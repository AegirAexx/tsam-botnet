// TODO: VALIDATION!!

#include "../../include/model/Command.h"

Command::Command(){}
Command::Command(std::string raw){
    this->raw = raw;
    this->commandID = u.idCommand(raw);

}

Command::~Command() {}

// Add a pretty print overload
std::ostream& operator << (std::ostream& outs, const Command& command){
    // Command tempMessage = message; // Temporary variable to circumvent "const" restrictions.
    // std::time_t t = static_cast<std::time_t>(tempMessage.timeStamp);
    // outs << "--\nTO:   " << tempMessage.recipient
    //      << "\nFROM: " << tempMessage.recipient
    //      << "\n--\nPAYLOAD:\n" << tempMessage.message
    //      << "\n--\nTIMESTAMP: " << std::put_time(std::gmtime(&t), "%d.%m.%y|%H:%M:%S"  )
    // << std::endl;
    return outs;
}