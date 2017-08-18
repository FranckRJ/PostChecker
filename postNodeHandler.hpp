#ifndef POSTNODEHANDLER_HPP
#define POSTNODEHANDLER_HPP

#include <list>
#include <memory> //std::shared_ptr
#include <unordered_set>
#include <string>

#include "postNode.hpp"

/**
 * @brief Enum indquant le status d'un @ref postNodeHandler.
 */
enum class handlerStatusEnum { NOT_STARTED, IN_PROGRESS, SUCCESS, NO_RESULT_FOUND };

/**
 * @brief Namespace contenant des fonctions aidants pour travailler avec les @ref handlerStatusEnum.
 */
namespace handlerStatusTool
{
    /**
     * @brief Retourne l'équivalent en string du status passé en paramètre.
     *
     * @param thisStatus Status à convertir en string.
     * @return Le status en tant que string lisible.
     */
    std::string statusToString(handlerStatusEnum thisStatus);
}

/**
 * @brief Classe gérant la vérification de la correspondance de Post.
 */
class postNodeHandler final
{
public:
    /**
     * @brief Constructeur de la classe.
     *
     * Aucune vérification n'est faite quant à la validité de @p newListOfAllPairs, si ce paramètre vaut @c null les autres méthodes crasheront.
     *
     * @param newListOfAllPairs Liste des paires de bases avec lesquelles la correspondance doit être vérifiée.
     */
    explicit postNodeHandler(std::list<postString>* newListOfAllPairs);

    /**
     * @brief Suppression des méthodes de copie car la classe contient des pointeurs.
     */
    postNodeHandler(const postNodeHandler&) = delete;

    /**
     * @brief Suppression des méthodes de copie car la classe contient des pointeurs.
     */
    postNodeHandler operator=(const postNodeHandler&) = delete;

    /**
     * @brief Teste si une correspondance est peut-être possible avec les paires de base.
     *
     * Retourne @c true si une combinaison de début est trouvée ainsi qu'une combinaison de fin, @c false sinon.
     * Doit être appelé une seule fois avant @ref makeNewNodesOrUpdateCurrents().
     *
     * @return @c true si la correspondance est peut-être possible, @c false sinon.
     */
    bool testIfCorrespondenceIsMaybePossible();

    /**
     * @brief Avance d'une étape dans la vérification de la correspondance de Post.
     *
     * Retourne @c true si une réponse à été trouvée, @c false sinon.
     * Utilisez @ref getStatus() pour connaitre la réponse.
     *
     * @return @c true si une réponse à été trouvée, @c false sinon.
     */
    bool makeNewNodesOrUpdateCurrents();

    /**
     * @brief Retourne la paire qui valide la correspondance de Post si trouvée.
     *
     * @return La paire qui valide la correspondance de Post si trouvée, sinon retourne une paire vide.
     */
    const postString& getSuccessPair() const;

    /**
     * @brief Retourne le status actuel de la recherche.
     *
     * @return Le status actuel de la recherche.
     */
    handlerStatusEnum getStatus() const;

    /**
     * @brief Retourne le nombre de node ayants timeout.
     *
     * Les nodes ayants timeout sont les nodes qui ont dépassés la profondeur maximale.
     *
     * @return Le nombre de node ayants timeout.
     * @see setMaxDepthForNodes()
     */
    int getNumberOfNodesTimeout() const;

    /**
     * @brief Retourne le nombre de node qui sont arrêtées pour s'être répétées.
     *
     * @return Le nombre de nodes qui sont arrêtées pour s'être répétées.
     * @see setCheckForRepeat()
     */
    int getNumberOfNodesRepeated() const;

    /**
     * @brief Retourne le nombre de node arrêtées sans résultat.
     *
     * Une node sans résultat est une node dont la différence entre les éléments de la paire est telle qu'il est impossible de lui ajouter une autre paire.
     * La différence entre les éléments de la paire n'est pas nulle sur cette node.
     *
     * @return Le nombre de node arrêtées sans résultat.
     */
    int getNumberOfNodesWithoutResult() const;

    /**
     * @brief Retourne le nombre de node arrêtées car la différence entre les éléments de la paire est trop grande.
     *
     * @return Le nombre de node arrêtées car la différence entre les éléments de la paire est trop grande.
     * @see setMaxDifferenceBetweenStringsSizeInPair()
     */
    int getNumberOfNodesStringDifferenceExceeded() const;

    /**
     * @brief Retourne la pronfondeur actuelle des nodes.
     *
     * @return La pronfondeur actuelle des nodes.
     */
    int getCurrentDepthForNodes() const;

    /**
     * @brief Retourne le nombre de node actuellement actives (non stoppées).
     *
     * @return Le nombre de node actuellement actives (non stoppées).
     */
    int getNumberOfNodesActives() const;

    /**
     * @brief Active ou désactive la vérification de répétition.
     *
     * Si la vérification est activée, les nodes se répétant seront stoppées.
     *
     * @param newVal @c true pour activer la vérification, @c false pour la désactiver.
     * @see getNumberOfNodesRepeated()
     */
    void setCheckForRepeat(bool newVal);

    /**
     * @brief Change la profondeur maximale des nodes.
     *
     * Si une node dépasse la profondeur maximale, elle sera stoppée.
     *
     * @param newVal Profondeur maximale des nodes, infini si inférieur ou égal à 0.
     * @see getNumberOfNodesTimeout()
     */
    void setMaxDepthForNodes(unsigned long newVal);

    /**
     * @brief Change la différence maximale entre les éléments de la paire.
     *
     * Si une node dépasse la différence maximale entre les éléments de la paire, elle sera stoppée.
     *
     * @param newVal Différence maximale entre les éléments de la paire, infini si inférieur ou égal à 0.
     * @see getNumberOfNodesStringDifferenceExceeded()
     */
    void setMaxDifferenceBetweenStringsSizeInPair(size_t newVal);
private:
    std::shared_ptr<postNode> createNewNode(postString thisPair, bool updateDifferenceEncountered, std::shared_ptr<postNode> thisParent = std::shared_ptr<postNode>());
    void updateNode(postNode& thisNode, postString thisPair, bool updateDifferenceEncountered);
    void eraseDifferenceOfCorrespondence(postNode& thisNode);
    bool checkIfCurrentDifferenceIsRepeated(const postNode& thisNode);
    bool checkIfNodeIsFinal(const postNode& thisNode);
    bool checkIfNodeReachLimits(const postNode& thisNode);
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
