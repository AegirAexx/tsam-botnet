// SECTION: LAYER THAT HANDELS READ / WRITE TO FILES.

#include <fstream>
#include <iostream>
#include <vector>

#include "LogData.h"

// TODO: BRING IN TIME
// TODO: DECIDE ON LOG INFO AND LAYOUT
// FEAT: Maybe we need a custom data type to hold the data in memory??

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