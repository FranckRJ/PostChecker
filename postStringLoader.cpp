#include <fstream>

#include "postStringLoader.hpp"

void postStringLoader::loadAllPairFromFile(const std::string& thisFile, std::list<postString>& toThisList)
{
    std::ifstream fileForPairs(thisFile);

    if(fileForPairs.is_open() == true)
    {
        std::string currentLine;
        postString currentPair;

        while(std::getline(fileForPairs, currentLine))
        {
            if(currentLine.empty() == false)
            {
                while(currentLine.back() == '\n' || currentLine.back() == '\r')
                {
                    currentLine.pop_back();
                }

                currentPair.first = currentLine.substr(0, currentLine.find(' '));
                currentPair.second = currentLine.substr(currentLine.find(' ') + 1);

                toThisList.push_back(currentPair);
            }
        }

        fileForPairs.close();
    }
}
