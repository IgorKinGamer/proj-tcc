#ifndef ID_H
#define ID_H

#include <bitset>
#include <climits>
#include <string>

typedef unsigned long ID;

std::string idBin(ID id, int bits = sizeof(ID)*CHAR_BIT)
{
	std::string repr = std::bitset<sizeof(ID)*CHAR_BIT>(id).to_string();
	return repr.substr(repr.size() - bits);
}

#endif /* ID_H */
