#ifndef POSTSTRINGLOADER_HPP
#define POSTSTRINGLOADER_HPP

#include <string>
#include <list>

#include "postNode.hpp"

/**
 * @brief Namespace contenant des fonctions permettant de charger une liste de paire.
 */
namespace postStringLoader
{
    /**
     * @brief Charge les paires du fichier @p thisFile dans la liste @p toThisList.
     *
     * Cette fonction peut avoir des comportements imprévus si le contenu du fichier n'est pas écrit de la bonne manière.
     *
     * @param thisFile Nom du fichier.
     * @param toThisList Liste de paire à remplir.
     */
    void loadAllPairFromFile(const std::string& thisFile, std::list<postString>& toThisList);
}

#endif
