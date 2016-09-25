#ifndef POSTNODEHANDLER_HPP
#define POSTNODEHANDLER_HPP

#include <list>
#include <memory> //std::shared_ptr
#include <unordered_set>

#include "postNode.hpp"

enum class handlerStatusEnum { NOT_STARTED, IN_PROGRESS, SUCCESS, ERROR };


/* Aucune verification n'est faite quant a la validite de "newListOfAllPairs", si ce parametre vaut nul
   les autres methodes crasheront. */
class postNodeHandler final
{
public:
    explicit postNodeHandler(std::list<postString>* newListOfAllPairs);

    /* Supression des methodes de copie car la classe contient des pointeurs. */
    postNodeHandler(const postNodeHandler&) = delete;
    postNodeHandler operator=(const postNodeHandler&) = delete;

    /* Retourne "true" si une combinaison de debut est trouve ainsi qu'une combinaison de fin, "false" sinon.
       Doit etre appele une seule fois avant makeNewNodesOrUpdateCurrents().*/
    bool testIfCorrespondenceIsMaybePossible();

    /* Retourne "true" si une reponse a ete trouvee, "false" sinon.
       Utilisez getStatus() pour connaitre la reponse. */
    bool makeNewNodesOrUpdateCurrents();

    const postString& getSuccessPair() const;
    handlerStatusEnum getStatus() const;
    int getNumberOfNodesTimeout() const;
    int getNumberOfNodesRepeated() const;
    int getNumberOfNodesWithoutResult() const;
    int getNumberOfNodesStringDifferenceExceeded() const;
    int getCurrentDepthForNodes() const;
    int getNumberOfNodesActives() const;
    void setCheckForRepeat(bool newVal);
    void setMaxDepthForNodes(unsigned long newVal);
    void setMaxDifferenceBetweenStringsSizeInPair(size_t newVal);
private:
    std::shared_ptr<postNode> createNewNode(postString thisPair, bool updateDifferenceEncountered, std::shared_ptr<postNode> thisParent = std::shared_ptr<postNode>());
    void updateNode(postNode& thisNode, postString thisPair, bool updateDifferenceEncountered);
    void eraseDifferenceOfCorrespondence(postNode& thisNode);
    bool checkIfCurrentDifferenceIsRepeated(const postNode& thisNode);
    bool checkIfNodeIsFinal(const postNode& thisNode);
    bool firstsCharsMatch(const postString& thisPair);
    bool lastsCharsMatch(const postString& thisPair);
    postString buildSuccessPairFromNode(const postNode& thisNode);
    postString getDifferenceFromPair(postString thisPair);
    size_t getStringSizeDifferenceFromPair(const postString& thisPair);
private:
    bool checkForRepeat = true;
    unsigned long maxDepthForNodes = 500;
    size_t maxDifferenceBetweenStringsSizeInPair = 20;
    std::list<std::shared_ptr<postNode>> listOfCurrentLeafs;
    std::unordered_set<postString, postString_hash> listOfAllDifferencesEncountered;
    std::list<postString>* listOfAllPairs = nullptr;
    handlerStatusEnum handlerStatus = handlerStatusEnum::NOT_STARTED;
    unsigned long currentDepthForNodes = 0;
    int numberOfNodesTimeout = 0;
    int numberOfNodesRepeated = 0;
    int numberOfNodesWithoutResult = 0;
    int numberOfNodesStringDifferenceExceeded = 0;
    postString successPair;
};

#endif
