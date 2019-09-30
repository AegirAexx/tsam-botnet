#ifndef COMMAND_H
#define COMMAND_H

// #include <ctime>
// #include <chrono>
// #include <cstring>
#include <ostream>
#include <vector>
// #include <iomanip>

class Command {
    public:
        Command(int commandID, std::vector<std::string> arguments);
        virtual ~Command();

        friend std::ostream& operator << (std::ostream& outs, const Command& command);

    private:
    int commandID;
    std::string first;
    std::string second;
    std::string third;


};

#endif // COMMAND_H

