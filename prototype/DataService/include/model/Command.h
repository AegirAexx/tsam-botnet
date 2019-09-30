#ifndef COMMAND_H
#define COMMAND_H

// #include <ctime>
// #include <chrono>
// #include <cstring>
#include <ostream>
#include "../service/Utilities.h"
// #include <iomanip>

class Command {
    public:
        Command();
        Command(std::string raw);
        // Command(...);
        virtual ~Command();

        friend std::ostream& operator << (std::ostream& outs, const Command& command);

    private:
    int commandID;
    std::string raw;
    std::string first;
    std::string second;
    std::string third;
    Utilities u;


};

#endif // COMMAND_H

