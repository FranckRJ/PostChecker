#ifndef POSTNODE_HPP
#define POSTNODE_HPP

#include <utility> //std::pair
#include <string>
#include <memory> //std::shared_ptr

typedef std::pair<std::string, std::string> postString;

postString operator+(const postString& param1, const postString& param2);

/*	Methode de hash pour pouvoir utiliser l'unordered_set. */
struct postString_hash
{
    std::size_t operator()(const postString& paramToHash) const;
};

struct postNode
{
	postNode() = default;

	/*	Supression des methodes de copie car la classe contient des pointeurs. */
	postNode(const postNode&) = delete;
	postNode operator=(const postNode&) = delete;

	std::shared_ptr<postNode> parentNode;
	postString currentDifference;
	postString currentCorrespondenceBuilded;
};

#endif
