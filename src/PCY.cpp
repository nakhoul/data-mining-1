#include "PCY.h"

#include <vector>
#include <map>
#include <set>

//once again, very fortunate to have sorted natural numbers in sets
//also a lot of reusable structures from Apriori
std::vector<std::pair<int, int>> PCY::findFrequentPairs(const std::vector<std::vector<int>>& transactions, int support) {
	
	//PCY: Count items + hash pairs and keep bucket counts >> bitmap >> find frequent pairs from candidates
	const int BUCKETS = 100000; //Some arbitray number; wanted to maybe have this change through each iteration
	std::map<int, int> itemCounts;
	std::vector<int> bucketCounts(BUCKETS, 0);

	for (const auto& basket : transactions) {
		for (int item : basket) itemCounts[item]++;


		for (int i = 0; i < basket.size(); i++) {
			for (int j = i + 1; j < basket.size(); j++) {
				int bucket = (41 * basket[i] + basket[j]) % BUCKETS; //arbitrary hash function

				bucketCounts[bucket]++;
			}
		}
	}

	std::set<int> frequentItems;

	for (const auto& [item, frequency] : itemCounts) {
		if (frequency >= support) {
			frequentItems.insert(item);
		}
	}

	// bitmap
	std::vector<bool> bitmap(BUCKETS);
	for (int i = 0; i < BUCKETS; i++) {
		if (bucketCounts[i] >= support) bitmap[i] = true;  //set
	}

	// SECOND PASS
	std::map<std::pair<int, int>, int> pairCounts;

	for (const auto& basket : transactions) {
		std::vector<int> filtered;

		for (int item : basket) {
			if (frequentItems.count(item)) filtered.push_back(item);
		}

		for (int i = 0; i < filtered.size(); i++) {
			for (int j = i + 1; j < filtered.size(); j++) {

				std::pair<int, int> p = { filtered[i], filtered[j] };

				int bucket = (41 * filtered[i] + filtered[j]) % BUCKETS;
				if (bitmap[bucket]) pairCounts[p]++;
			}
		}
	}


	std::vector<std::pair<int, int>> frequentPairs;

	for (const auto& [pair, frequency] : pairCounts) {
		if (frequency >= support) frequentPairs.push_back(pair);
	}
	
	return frequentPairs;
}

//tried rewriting with unordered_maps and a couple different optimizations, but didn't affect performance vs Apriori that much