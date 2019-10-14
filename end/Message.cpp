// COM: T-409-TSAM-2019-3 | Project 3 - The Botnet Rises
// COM: Aegir_Tomasson[aegir15] && Dagur_Kristjansson[dagur17]


// COM: This is our datatype to hold messages and log to file.
// COM:
// COM: Message API:
// COM:  public:
// COM:      Message(std::string from, std::string to, std::string msg) - Class constructor, takes the buffer.
// COM:      virtual ~Message() - Class destructor.
// COM:
// COM:      string getFrom() - Class getter.
// COM:      string getTo() - Class getter.
// COM:      string getMsg() - Class getter.
// COM:      size_t getTimeStamp() - Class getter.
// COM:      bool getIsSend() - Class getter.
// COM:      bool getIsClient() - Class getter.
// COM:      string getFormattedMessage() - Class getter.
// COM:      int getGroupID() - Class getter.
// COM:
// COM:      void logMessage(int id) - File stream to write log.
// COM:
// COM:  private:
// COM:      string from - Message sender.
// COM:      string to - Message recipient.
// COM:      string msg - Message payload.
// COM:      int groupID - Recipient identifier.
// COM:      size_t timeStamp - Seconds since Epoch.
// COM:      bool isClient - Flag for logging.
// COM:      bool isSend - Flag for logging.
// COM:
// COM:      vector<string> split(string stringToSplit, char delimeter) - Private utility function.


#include "Message.h"

// COM: Class constructor.
Message::Message(std::string from, std::string to, std::string msg) {
    this->from = from;
    this->to = to;
    this->msg = msg;
    // COM: Timestamp - seconds since Epoch.
    this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    this->groupID = getGroupID();
    // COM: Render flags.
    this->isClient = false;
    this->isSend = false;
}

// COM: Class destructor.
Message::~Message() {}


// COM: Utility function that splits a string according to a delimiter and returns a vector of single words or tokens.
std::vector<std::string> Message::split(std::string stringToSplit, char delimeter) {
    // COM: Creating a string stream object from input string.
    std::stringstream ss(stringToSplit);
    // COM: Temporary placeholder.
    std::string word;
    // COM: Container to hold words or tokens.
	std::vector<std::string> splittedStrings;
    // COM: While getline is fed strings from the string stream object, they are push onto the container.
    while (std::getline(ss, word, delimeter)) splittedStrings.push_back(word);
    // COM: The container is returned.
    return splittedStrings;
}

// COM: Class getters.
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

// COM: Function with fstream access and logic to log messages to text file.
void Message::logMessage(int id) {
    // COM: Logic to render logs correctly.
    if(id == 1 || id == 11) this->isSend = true;
    if(id == 2 || id == 10) this->isSend = false;
    if(id == 10 || id == 11) this->isClient = true;
    if(id == 2 || id == 1) this->isClient = false;
    // COM: Seconds since Epoch
    if(id == 2 || id == 10) this->timeStamp = (size_t)std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // COM: Writing to text file.
    std::ofstream outStream;
    outStream.open("logs/MessageLog.txt", std::ios::app);
    if(outStream.is_open()) outStream << *this;
    outStream.close();
}

// COM: Overwriting the << operator to format output to file.
std::ostream& operator << (std::ostream& outs, const Message& msg) {
    Message tempMessage = msg;
    // COM: Time object for static casting seconds since Epoch.
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

