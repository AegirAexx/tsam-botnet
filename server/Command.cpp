// TODO: VALIDATION!!

#include "Command.h"

Command::Command(int commandID, std::string arguments) {
    this->commandID = commandID;
    memset (this->buffer, 0, 4096);
    strcpy (this->buffer, arguments.c_str());
    this->argCount = 0;
}

// Add a pretty print overload
std::ostream& operator << (std::ostream& outs, const Command& command){
    Command tempCommand = command; // Temporary variable to circumvent "const" restrictions.
    outs << "Command#" << tempCommand.commandID << "\nBuffer: " << tempCommand.buffer << std::endl;
    return outs;
}
