#include <iostream> //std::cout
#include <list>
#include <string>
#include <cstdlib> //std::strtoul

#include "postNodeHandler.hpp"
#include "postNode.hpp"
#include "postStringLoader.hpp"

int main(int argc, char* argv[])
{
    std::list<postString> listOfAllPairs;
    bool printInfoAtEveryDepth = false;
    std::string textFile = "pcp.txt";
    postNodeHandler myHandler(&listOfAllPairs);

    if(argc > 1)
    {
        textFile = argv[1];
    }
    if(argc > 2)
    {
        const char valueChar = argv[2][0];

        myHandler.setCheckForRepeat(valueChar == 't' || valueChar == 'T');
    }
    if(argc > 3)
    {
        myHandler.setMaxDepthForNodes(std::strtoul(argv[3], nullptr, 10));
    }
    if(argc > 4)
    {
        myHandler.setMaxDifferenceBetweenStringsSizeInPair(std::strtoul(argv[4], nullptr, 10));
    }
    if(argc > 5)
    {
        const char valueChar = argv[5][0];

        printInfoAtEveryDepth = (valueChar == 't' || valueChar == 'T');
    }

    postStringLoader::loadAllPairFromFile(textFile, listOfAllPairs);

    std::cout << "Liste de toutes les paires :" << std::endl;
    for(const postString& thisPair : listOfAllPairs)
    {
        std::cout << thisPair.first << " " <<thisPair.second << std::endl;
    }
    std::cout << std::endl;

    if(myHandler.testIfCorrespondenceIsMaybePossible() == true)
    {
        std::cout << "Premiere verification validee." << std::endl << std::endl;

        if(myHandler.getStatus() == handlerStatusEnum::IN_PROGRESS)
        {
            while(myHandler.makeNewNodesOrUpdateCurrents() == false)
            {
                if(printInfoAtEveryDepth == true)
                {
                    std::cout << "Niveau : " << myHandler.getCurrentDepthForNodes();
                    std::cout << ", nombre de nodes actives : " << myHandler.getNumberOfNodesActives() << std::endl;
                }
            }
        }
        std::cout << "Niveau final : " << myHandler.getCurrentDepthForNodes();
        std::cout << ", nombre de nodes actives : " << myHandler.getNumberOfNodesActives() << std::endl << std::endl;

        std::cout << "Termine, status : " << handlerStatusTool::statusToString(myHandler.getStatus()) << std::endl;

        if(myHandler.getStatus() == handlerStatusEnum::SUCCESS)
        {
            std::cout << "Resultat :" << std::endl;
            std::cout << myHandler.getSuccessPair().first << std::endl;
            std::cout << myHandler.getSuccessPair().second << std::endl;
        }
        std::cout << std::endl;

        std::cout << "Statut des nodes :" << std::endl;
        std::cout << "Nombre de nodes sans resultat : " << myHandler.getNumberOfNodesWithoutResult() << std::endl;
        std::cout << "Nombre de nodes qui se repetent : " << myHandler.getNumberOfNodesRepeated() << std::endl;
        std::cout << "Nombre de nodes arrete car trop profondes : " << myHandler.getNumberOfNodesTimeout() << std::endl;
        std::cout << "Nombre de nodes arrete car difference entre chaines trop grande : " << myHandler.getNumberOfNodesStringDifferenceExceeded() << std::endl << std::endl;
    }
    else
    {
        std::cout << "Premiere verification non validee, impossible de trouver un resultat." << std::endl << std::endl;
    }

    return 0;
}
