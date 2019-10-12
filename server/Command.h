#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include <regex>
#include <iterator>

class Command {
    public:
        Command(std::string buffer);

        int getID();
        std::vector<std::string> getPayload();

    private:
        int id;
        std::vector<std::string> payload;
        std::vector<std::string> split(std::string stringToSplit, char delimeter);

};

#endif // COMMAND_H

