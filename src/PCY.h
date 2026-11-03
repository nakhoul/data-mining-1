#pragma once
#define pcy_h

#include "Algorithm.h"

class PCY : public Algorithm {
public:
	std::vector<std::pair<int, int>> findFrequentPairs(const std::vector<std::vector<int>>& transactions, int support) override;

};