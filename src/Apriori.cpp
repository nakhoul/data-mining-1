#include "Apriori.h"

#include <map>
#include <set> 
// All the lines in retail.txt are basically sorted sets, so this is way easier than it should've been

std::vector<std::pair<int, int>> Apriori::findFrequentPairs(const std::vector<std::vector<int>>& transactions, int support) {

	// Standard-ish Apriori: frequent items >> candidate pair construction >> frequent pairs
	std::map<int, int> itemCounts;
	std::map<std::pair<int, int>, int> pairCounts;

	for (const auto& basket: transactions) {
		for (int item : basket) itemCounts[item]++;
	}

	std::set<int> frequentItems;

	for (const auto& [item, frequency] : itemCounts) {
		if (frequency >= support) {
			frequentItems.insert(item);
		}
	}

	for (const auto &basket: transactions) {
		std::vector<int> filtered;

		for (int item : basket) {
			if (frequentItems.count(item)) filtered.push_back(item);
		}

		for (int i = 0; i < filtered.size(); i++) {
			for (int j = i + 1; j < filtered.size(); j++) {

				std::pair<int, int> p = { filtered[i], filtered[j] };
				pairCounts[p]++;
			}
		}
	}

	std::vector<std::pair<int, int>> frequentPairs;

	for (const auto& [pair, frequency] : pairCounts) {
		if (frequency >= support) frequentPairs.push_back(pair);
	}

	return frequentPairs;

}