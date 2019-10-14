// T-409-TSAM-2019-3 | Project 3 - The Botnet Rises
// Aegir_Tomasson[aegir15] && Dagur_Kristjansson[dagur17]


// This is our datatype to hold messages and log to file.
//
// Message API:
//  public:
//      Message(std::string from, std::string to, std::string msg) - Class constructor, takes the buffer.
//      virtual ~Message() - Class destructor.
//
//      string getFrom() - Class getter.
//      string getTo() - Class getter.
//      string getMsg() - Class getter.
//      size_t getTimeStamp() - Class getter.
//      bool getIsSend() - Class getter.
//      bool getIsClient() - Class getter.
//      string getFormattedMessage() - Class getter.
//      int getGroupID() - Class getter.
//
//      void logMessage(int id) - File stream to write log.
//
//  private:
//      string from - Message sender.
//      string to - Message recipient.
//      string msg - Message payload.
//      int groupID - Recipient identifier.
//      size_t timeStamp - Seconds since Epoch.
//      bool isClient - Flag for logging.
//      bool isSend - Flag for logging.
//
//      vector<string> split(string stringToSplit, char delimeter) - Private utility function.


#include "Message.h"

// Class constructor.
Message::Message(std::string from, std::string to, std::string msg) {
    this->from = from;
    this->to = to;
    this->msg = msg;
    // Timestamp - seconds since Epoch.
    this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    this->groupID = getGroupID();
    // Render flags.
    this->isClient = false;
    this->isSend = false;
}

// Class destructor.
Message::~Message() {}


// Utility function that splits a string according to a delimiter and returns a vector of single words or tokens.
std::vector<std::string> Message::split(std::string stringToSplit, char delimeter) {
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
int Message::getGroupID() {
    auto temp = split(this->to, '_');
    return atoi(temp[temp.size()-1].c_str());
}

std::string Message::getFormattedMessage() {
    return this->from + "," + this->to + "," + this->msg;
}

std::string Message::getFrom() {
    return this->from;
}

std::string Message::getTo() {
    return this->to;
}

std::string Message::getMsg() {
    return this->msg;
}

size_t Message::getTimeStamp() {
    return this->timeStamp;
}

bool Message::getIsSend() {
    return this->isSend;
}

bool Message::getIsClient() {
    return this->isClient;
}

// Function with fstream access and logic to log messages to text file.
void Message::logMessage(int id) {
    // Logic to render logs correctly.
    if(id == 1 || id == 11) this->isSend = true;
    if(id == 2 || id == 10) this->isSend = false;
    if(id == 10 || id == 11) this->isClient = true;
    if(id == 2 || id == 1) this->isClient = false;
    // Seconds since Epoch
    if(id == 2 || id == 10) this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // Writing to text file.
    std::ofstream outStream;
    outStream.open("logs/MessageLog.txt", std::ios::app);
    if(outStream.is_open()) outStream << *this;
    outStream.close();
}

// Overwriting the << operator to format output to file.
std::ostream& operator << (std::ostream& outs, const Message& msg) {
    Message tempMessage = msg;
    // Time object for static casting seconds since Epoch.
    std::time_t t = static_cast<std::time_t>(tempMessage.timeStamp);
    outs << "COMMAND: " <<  (tempMessage.isSend ? "SEND" : "GET")
         << " | isBot: " << (tempMessage.isClient ? "TRUE" : "FALSE")
         << " | FROM: " << tempMessage.from
         << " | TO: " << tempMessage.to
         << " | MSG: " << tempMessage.msg
         << " | TIMESTAMP: " << std::put_time(std::gmtime(&t), "%d.%m.%y -=- %H:%M:%S")
    << std::endl;
    return outs;
}

