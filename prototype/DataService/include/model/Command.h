#ifndef COMMAND_H
#define COMMAND_H

// #include <ctime>
// #include <chrono>
#include <cstring>
#include <ostream>
#include <vector>
// #include <iomanip>

class Command {
    public:
        Command(int commandID, std::string arguments);

        friend std::ostream& operator << (std::ostream& outs, const Command& command);

        // Get argument [number] / Gets args from buffer.
        // std::string first;
        // std::string second;
        // std::string third;

    private:
        int commandID;
        int argCount;
        char buffer[4096];



};

#endif // COMMAND_H

