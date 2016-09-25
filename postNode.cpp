#include "postNode.hpp"

postString operator+(const postString& param1, const postString& param2)
{
    postString newPair;

    newPair.first = param1.first + param2.first;
    newPair.second = param1.second + param2.second;

    return newPair;
}

std::size_t postString_hash::operator()(const postString& paramToHash) const
{
    std::string newString = paramToHash.first + " " + paramToHash.second;
    return std::hash<std::string>{}(newString);
}
