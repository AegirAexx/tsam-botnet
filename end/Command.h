// T-409-TSAM-2019-3 | Project 3 - The Botnet Rises
// Aegir_Tomasson[aegir15] && Dagur_Kristjansson[dagur17]


#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include <regex>
#include <iterator>

// This is our datatype to hold and identify commands as they come off the buffer.

class Command {
    public:
    // Class constructor / Destructor.
        Command(std::string buffer);
        virtual ~Command();

    // Class getters.
        int getID();
        std::vector<std::string> getPayload();

    private:
    // Class variables.
        int id;
        std::vector<std::string> payload;
    // Class utility
        std::vector<std::string> split(std::string stringToSplit, char delimeter);

};

#endif // COMMAND_H

