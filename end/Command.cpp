// T-409-TSAM-2019-3 | Project 3 - The Botnet Rises
// Aegir_Tomasson[aegir15] && Dagur_Kristjansson[dagur17]


// This is our datatype to hold and identify commands as they come off the buffer.
//
// Command API:
//  public:
//      Command(string buffer) - Class constructor, takes the buffer.
//      virtual ~Command() - Class destructor.
//
//      int getID() - Class getter.
//      vector<string> getPayload() - Class getter.
//
//  private:
//      int id - Command identifier.
//      vector<string> payload - The buffer without the command.
//
//      vector<string> split(string stringToSplit, char delimeter) - Private utility function.


#include "Command.h"

// Class constructor.
Command::Command(std::string buffer) {
    // Regular expression that catches everything before the first comma.
    std::regex rx("^[\\w\\s]+(?=,)");
    // Smatch object to hold iterators for potential matches.
    std::smatch match;
    // Regex attempts to match a regular expression to any part of a character sequence.
    std::regex_search(buffer, match, rx);

    // Were there any matches?
    if(!match.empty()){
        // Temporary placeholder for matched string.
        std::string command(match.str());
        // Normalize buffer in anonymous function that returns upper case.
        std::transform(command.begin(), command.end(), command.begin(), [] (unsigned char c){ return std::toupper(c); });
        // Determine the command.
        if(command == "SEND_MSG") this->id = 1;
        else if(command == "GET_MSG") this->id = 2;
        else if(command == "KEEPALIVE") this->id = 3;
        else if(command == "CONNECT") this->id = 4;
        else if(command == "LEAVE") this->id = 5;
        else if(command == "STATUSREQ") this->id = 6;
        else if(command == "LISTSERVERS") this->id = 7;
        else if(command == "SERVERS") this->id = 8;
        else if(command == "STATUSRESP") this->id = 9;
        else if(command == "GETMSG") this->id = 10;
        else if(command == "SENDMSG") this->id = 11;
        // If it's not a known command but is formated like the protocol suggests.
        else this->id = 0;
    // If the buffer does not contain anything close to valid.
    } else this->id = -1;

    // Process the buffer into a workable payload.
    if(this->id > 0) {
        // Regular expression that catches every semicolon.
        std::regex rx(";");
        // Calling split on the returned regex_replace string. It now only has commas as its delimiter.
        this->payload = split(std::regex_replace(buffer,rx,","), ',');
        // Remove the command off the payload.
        this->payload.erase(this->payload.begin());
    }
}


// Class Destructor.
Command::~Command() {};


// Utility function that splits a string according to a delimiter and returns a vector of single words or tokens.
std::vector<std::string> Command::split(std::string stringToSplit, char delimeter) {
    // Creating a string stream object from input string.
    std::stringstream ss(stringToSplit);
    // Temporary placeholder.
    std::string word;
    // Container to hold words or tokens.
	std::vector<std::string> splittedStrings;
    // While getline is fed strings from the string stream object, they are push onto the container.
    while (std::getline(ss, word, delimeter)) splittedStrings.push_back(word);
    // The container is returned.
    return splittedStrings;
}

// Class getters.
std::vector<std::string>  Command::getPayload(){
    return this->payload;
}

int Command::getID(){
    return this->id;
}