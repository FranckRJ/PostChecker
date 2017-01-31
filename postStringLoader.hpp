#ifndef POSTSTRINGLOADER_HPP
#define POSTSTRINGLOADER_HPP

#include <string>
#include <list>

#include "postNode.hpp"

namespace postStringLoader
{
    /* Cette fonction peut avoir des comportements imprevus si le contenu du fichier n'est pas ecrit de la bonne maniere. */
    void loadAllPairFromFile(const std::string& thisFile, std::list<postString>& toThisList);
}

#endif
