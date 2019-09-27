// SECTION: LAYER THAT HANDELS READ / WRITE TO FILES.

#include <fstream>
#include <iostream>
#include <vector>

// INCLUDE HEADER FOR TYPES


int main(){

    std::cout << "The data layer..." << std::endl;

    // PART: INPUT FROM FILE

    std::vector<std::string> logs;

    std::string lineLog;

    std::ifstream messageLogs;

    messageLogs.open("logs.txt");

    while (std::getline(messageLogs, lineLog)){
        logs.push_back(lineLog);
    }

    messageLogs.close();

    // PART: OUTPUT TO FILE



    return 0;
}