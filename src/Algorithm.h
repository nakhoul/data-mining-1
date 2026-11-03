#pragma once
#define algorithm_h

#include <vector>
#include <utility>

class Algorithm {
public:
	virtual ~Algorithm() = default;
	virtual std::vector<std::pair<int, int>> findFrequentPairs(const std::vector<std::vector<int>>& transactions, int support) = 0;
};