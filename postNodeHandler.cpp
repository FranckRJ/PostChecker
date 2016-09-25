#include <algorithm> //std::min
#include <cmath> //std::abs

#include "postNodeHandler.hpp"

postNodeHandler::postNodeHandler(std::list<postString>* newListOfAllPairs) :
                 listOfAllPairs(newListOfAllPairs)
{
}

bool postNodeHandler::testIfCorrespondenceIsMaybePossible()
{
    bool matchBegining = false;
    bool matchEnding = false;

    for(const postString& thisPair : *listOfAllPairs)
    {
        if(thisPair.first.empty() == true || thisPair.second.empty() == true)
        {
            return false;
        }

        if(firstsCharsMatch(thisPair) == true)
        {
            matchBegining = true;
            if(checkForRepeat == false || listOfAllDifferencesEncountered.count(getDifferenceFromPair(thisPair)) == 0)
            {
                listOfCurrentLeafs.push_back(createNewNode(thisPair, true));

                if(checkIfNodeIsFinal(*listOfCurrentLeafs.back()) == true)
                {
                    successPair = buildSuccessPairFromNode(*listOfCurrentLeafs.back());
                    handlerStatus = handlerStatusEnum::SUCCESS;
                    return true;
                }
            }
        }
        if(lastsCharsMatch(thisPair) == true)
        {
            matchEnding = true;
        }
    }

    if(matchBegining == true && matchEnding == true)
    {
        handlerStatus = handlerStatusEnum::IN_PROGRESS;
        return true;
    }

    return false;
}

bool postNodeHandler::makeNewNodesOrUpdateCurrents()
{
    ++currentDepthForNodes;

    for(std::list<std::shared_ptr<postNode>>::iterator ite = listOfCurrentLeafs.begin(); ite != listOfCurrentLeafs.end(); )
    {
        std::list<postString> listOfCompatiblePair;

        for(const postString& thisPair : *listOfAllPairs)
        {
            if(firstsCharsMatch((*ite)->currentDifference + thisPair) == true)
            {
                listOfCompatiblePair.push_back(thisPair);
            }
        }

        if(listOfCompatiblePair.empty() == true)
        {
            ite = listOfCurrentLeafs.erase(ite);
            ++numberOfNodesWithoutResult;
            continue;
        }
        else if(listOfCompatiblePair.size() == 1)
        {
            updateNode(**ite, listOfCompatiblePair.back(), false);

            if(checkIfNodeIsFinal(**ite) == true)
            {
                handlerStatus = handlerStatusEnum::SUCCESS;
                successPair = buildSuccessPairFromNode(**ite);
                return true;
            }

            if(checkForRepeat == true)
            {
                if(checkIfCurrentDifferenceIsRepeated(**ite) == true)
                {
                    ite = listOfCurrentLeafs.erase(ite);
                    ++numberOfNodesRepeated;
                    continue;
                }
                else
                {
                    listOfAllDifferencesEncountered.insert((*ite)->currentDifference);
                }
            }
            if(maxDifferenceBetweenStringsSizeInPair > 0)
            {
                if(getStringSizeDifferenceFromPair((*ite)->currentDifference) > maxDifferenceBetweenStringsSizeInPair)
                {
                    ite = listOfCurrentLeafs.erase(ite);
                    ++numberOfNodesStringDifferenceExceeded;
                    continue;
                }
            }
        }
        else
        {
            eraseDifferenceOfCorrespondence(**ite);

            for(const postString& thisPair : listOfCompatiblePair)
            {
                std::list<std::shared_ptr<postNode>>::iterator newIte;
                newIte = listOfCurrentLeafs.insert(listOfCurrentLeafs.begin(), createNewNode((*ite)->currentDifference + thisPair, false, *ite));

                if(checkIfNodeIsFinal(**newIte) == true)
                {
                    handlerStatus = handlerStatusEnum::SUCCESS;
                    successPair = buildSuccessPairFromNode(**newIte);
                    return true;
                }

                if(checkForRepeat == true)
                {
                    if(checkIfCurrentDifferenceIsRepeated(**newIte) == true)
                    {
                        listOfCurrentLeafs.erase(newIte);
                        ++numberOfNodesRepeated;
                    }
                    else
                    {
                        listOfAllDifferencesEncountered.insert((*newIte)->currentDifference);
                    }
                }
                if(maxDifferenceBetweenStringsSizeInPair > 0)
                {
                    if(getStringSizeDifferenceFromPair((*newIte)->currentDifference) > maxDifferenceBetweenStringsSizeInPair)
                    {
                        listOfCurrentLeafs.erase(newIte);
                        ++numberOfNodesStringDifferenceExceeded;
                    }
                }
            }

            ite = listOfCurrentLeafs.erase(ite);
            continue;
        }

        ++ite;
    }

    if(maxDepthForNodes != 0 && currentDepthForNodes >= maxDepthForNodes)
    {
        handlerStatus = handlerStatusEnum::ERROR;
        numberOfNodesTimeout = listOfCurrentLeafs.size();
        return true;
    }
    if(listOfCurrentLeafs.empty() == true)
    {
        handlerStatus = handlerStatusEnum::ERROR;
        return true;
    }

    return false;
}

const postString& postNodeHandler::getSuccessPair() const
{
    return successPair;
}

handlerStatusEnum postNodeHandler::getStatus() const
{
    return handlerStatus;
}

int postNodeHandler::getNumberOfNodesTimeout() const
{
    return numberOfNodesTimeout;
}

int postNodeHandler::getNumberOfNodesRepeated() const
{
    return numberOfNodesRepeated;
}

int postNodeHandler::getNumberOfNodesWithoutResult() const
{
    return numberOfNodesWithoutResult;
}

int postNodeHandler::getNumberOfNodesStringDifferenceExceeded() const
{
    return numberOfNodesStringDifferenceExceeded;
}

int postNodeHandler::getCurrentDepthForNodes() const
{
    return currentDepthForNodes;
}

int postNodeHandler::getNumberOfNodesActives() const
{
    return listOfCurrentLeafs.size();
}

void postNodeHandler::setCheckForRepeat(bool newVal)
{
    checkForRepeat = newVal;
}

void postNodeHandler::setMaxDepthForNodes(unsigned long newVal)
{
    maxDepthForNodes = newVal;
}

void postNodeHandler::setMaxDifferenceBetweenStringsSizeInPair(size_t newVal)
{
    maxDifferenceBetweenStringsSizeInPair = newVal;
}

std::shared_ptr<postNode> postNodeHandler::createNewNode(postString thisPair, bool updateDifferenceEncountered, std::shared_ptr<postNode> thisParent)
{
    std::shared_ptr<postNode> newNode(std::make_shared<postNode>());

    newNode->currentCorrespondenceBuilded = thisPair;
    newNode->parentNode = thisParent;
    newNode->currentDifference = getDifferenceFromPair(newNode->currentCorrespondenceBuilded);

    if(updateDifferenceEncountered == true && checkForRepeat == true)
    {
        listOfAllDifferencesEncountered.insert(newNode->currentDifference);
    }

    return newNode;
}

void postNodeHandler::updateNode(postNode& thisNode, postString thisPair, bool updateDifferenceEncountered)
{
    thisNode.currentCorrespondenceBuilded = thisNode.currentCorrespondenceBuilded + thisPair;
    thisNode.currentDifference = getDifferenceFromPair(thisNode.currentCorrespondenceBuilded);

    if(updateDifferenceEncountered == true && checkForRepeat == true)
    {
        listOfAllDifferencesEncountered.insert(thisNode.currentDifference);
    }
}

void postNodeHandler::eraseDifferenceOfCorrespondence(postNode& thisNode)
{
    postString& tmpCorres = thisNode.currentCorrespondenceBuilded;
    postString& tmpDiff = thisNode.currentDifference;

    if(tmpDiff.first.empty() == false)
    {
        tmpCorres.first.erase(tmpCorres.first.size() - tmpDiff.first.size(), tmpDiff.first.size());
    }
    if(tmpDiff.second.empty() == false)
    {
        tmpCorres.second.erase(tmpCorres.second.size() - tmpDiff.second.size(), tmpDiff.second.size());
    }
}

bool postNodeHandler::checkIfCurrentDifferenceIsRepeated(const postNode& thisNode)
{
    return (listOfAllDifferencesEncountered.count(thisNode.currentDifference) > 0);
}

bool postNodeHandler::checkIfNodeIsFinal(const postNode& thisNode)
{
    return (thisNode.currentDifference.first.empty() == true && thisNode.currentDifference.second.empty() == true);
}

bool postNodeHandler::firstsCharsMatch(const postString& thisPair)
{
    int minimumSize = std::min(thisPair.first.size(), thisPair.second.size());

    for(int i = 0; i < minimumSize; ++i)
    {
        if(thisPair.first.at(i) != thisPair.second.at(i))
        {
            return false;
        }
    }

    return true;
}

bool postNodeHandler::lastsCharsMatch(const postString& thisPair)
{
    int minimumSize = std::min(thisPair.first.size(), thisPair.second.size());

    for(int i = 0; i < minimumSize; ++i)
    {
        if(thisPair.first.at(thisPair.first.size() - i - 1) != thisPair.second.at(thisPair.second.size() - i - 1))
        {
            return false;
        }
    }

    return true;
}

postString postNodeHandler::buildSuccessPairFromNode(const postNode& thisNode)
{
    postString newSuccessPair;
    const postNode* nodeToCheck = &thisNode;

    while(nodeToCheck != nullptr)
    {
        newSuccessPair = nodeToCheck->currentCorrespondenceBuilded + newSuccessPair;

        nodeToCheck = nodeToCheck->parentNode.get();
    }

    return newSuccessPair;
}

postString postNodeHandler::getDifferenceFromPair(postString thisPair)
{
    int minimumSize = std::min(thisPair.first.size(), thisPair.second.size());
    int i = 0;

    while(i < minimumSize && thisPair.first.front() == thisPair.second.front())
    {
        thisPair.first.erase(0, 1);
        thisPair.second.erase(0, 1);
        ++i;
    }

    return thisPair;
}

/* Le cast en long long est pour autoriser une operation du type "2 - 5", car de base c'est un unsigned qui est retourne. */
size_t postNodeHandler::getStringSizeDifferenceFromPair(const postString& thisPair)
{
    return std::abs(static_cast<long long>(thisPair.first.size()) - static_cast<long long>(thisPair.second.size()));
}
