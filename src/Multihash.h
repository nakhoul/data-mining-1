#pragma once
#define multihash_h

#include "Algorithm.h"

class Multihash : public Algorithm {
public:
	std::vector<std::pair<int, int>> findFrequentPairs(const std::vector<std::vector<int>>& transactions, int support) override;

};