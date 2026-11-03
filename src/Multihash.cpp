#include "Multihash.h"

#include <vector>
#include <map>
#include <set>

//Basically extending base PCY
std::vector<std::pair<int, int>> Multihash::findFrequentPairs(const std::vector<std::vector<int>>& transactions, int support) {

	const int BUCKETS = 100000;
	std::map<int, int> itemCounts;

	//two hash tables
	std::vector<int> bucketCounts1(BUCKETS, 0);
	std::vector<int> bucketCounts2(BUCKETS, 0);

	for (const auto& basket : transactions) {
		for (int item : basket) itemCounts[item]++;


		for (int i = 0; i < basket.size(); i++) {
			for (int j = i + 1; j < basket.size(); j++) {

				//two distinct hash functions
				int bucket1 = (31 * basket[i] + 17 * basket[j]) % BUCKETS;
				int bucket2 = (13 * basket[i] + 47 * basket[j]) % BUCKETS;

				bucketCounts1[bucket1]++;
				bucketCounts2[bucket2]++;
			}
		}
	}

	std::set<int> frequentItems;

	for (const auto& [item, frequency] : itemCounts) {
		if (frequency >= support) {
			frequentItems.insert(item);
		}
	}


	//two bitmaps
	std::vector<bool> bitmap1(BUCKETS);
	std::vector<bool> bitmap2(BUCKETS);

	for (int i = 0; i < BUCKETS; i++) {
		if (bucketCounts1[i] >= support) bitmap1[i] = true;
		if (bucketCounts2[i] >= support) bitmap2[i] = true;
	}

	std::map<std::pair<int, int>, int> pairCounts;

	for (const auto& basket : transactions) {
		std::vector<int> filtered;

		for (int item : basket) {
			if (frequentItems.count(item)) filtered.push_back(item);
		}

		for (int i = 0; i < filtered.size(); i++) {
			for (int j = i + 1; j < filtered.size(); j++) {

				int bucket1 = (31 * filtered[i] + 17 * filtered[j]) % BUCKETS;
				int bucket2 = (13 * filtered[i] + 47 * filtered[j]) % BUCKETS;

				if (bitmap1[bucket1] && bitmap2[bucket2]) {
					std::pair<int, int> p = { filtered[i], filtered[j] };
					pairCounts[p]++;
				}
			}
		}
	}


	std::vector<std::pair<int, int>> frequentPairs;

	for (const auto& [pair, frequency] : pairCounts) {
		if (frequency >= support) frequentPairs.push_back(pair);
	}

	return frequentPairs;
}


//It's looking rough for PCYs
//Probably way too much overhead from a half-baked implementation
//Originally tried (41i + j) mod 100000 & (37i + 3j) mod 100000
//New hash function is slightly better, but this really isn't beating Apriori
