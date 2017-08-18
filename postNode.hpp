#ifndef POSTNODE_HPP
#define POSTNODE_HPP

#include <utility> //std::pair
#include <string>
#include <memory> //std::shared_ptr

/**
 * @brief Paire de string pour la correspondance de Post.
 */
typedef std::pair<std::string, std::string> postString;

/**
 * @brief Opérateur d'addition de deux paires.
 *
 * @param Première paire.
 * @param Seconde paire.
 * @return Une paire dont chaque membre est l'addition des membres équivalents des paires passées en paramètre.
 */
postString operator+(const postString& param1, const postString& param2);

/**
 * @brief Structure ajoutant une fonction de hash aux paires pour pouvoir utiliser l'unordered_set.
 */
struct postString_hash
{
    /**
     * @brief Fonction de hash.
     *
     * @param paramToHash Paire à hasher.
     * @return Un hash de la paire.
     */
    std::size_t operator()(const postString& paramToHash) const;
};

/**
 * @brief Structure représentant un état d'une paire.
 */
struct postNode
{
    /**
     * @brief Constructeur par défaut.
     */
    postNode() = default;

    /**
     * @brief Suppression des méthodes de copie car la classe contient des pointeurs.
     */
    postNode(const postNode&) = delete;

    /**
     * @brief Suppression des méthodes de copie car la classe contient des pointeurs.
     */
    postNode operator=(const postNode&) = delete;

    /**
     * @brief Un état précédent de la paire actuelle.
     */
    std::shared_ptr<postNode> parentNode;

    /**
     * @brief La différence actuelle entre les deux éléments de la paire.
     */
    postString currentDifference;

    /**
     * @brief La paire actuelle.
     */
    postString currentCorrespondenceBuilded;
};

#endif
