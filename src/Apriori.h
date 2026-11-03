#pragma once
#define apriori_h

#include "Algorithm.h"

class Apriori : public Algorithm {
public:
	std::vector<std::pair<int, int>> findFrequentPairs(const std::vector<std::vector<int>>& transactions, int support) override;

};