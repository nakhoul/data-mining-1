// Stats in report
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <chrono>
#include <cmath> //ceil
//#include <algorithm>
#include <cxxabi.h> //remove later

#include "Algorithm.h"
#include "Apriori.h"
#include "PCY.h"
#include "Multihash.h"

// input helper function
std::vector<std::vector<int>> load(const std::string& filename) {
	
	std::vector<std::vector<int>> transactions;
	std::ifstream file(filename);

	std::string line;

	while (getline(file, line)) {
		std::stringstream s(line);
		std::vector<int> basket;
		int item;

		while (s >> item) basket.push_back(item);

		if (!basket.empty()) transactions.push_back(basket);
	}

	return transactions;
}


int main() {
	std::vector<std::vector<int>> transactions = load("retail.txt");

	std::vector<std::unique_ptr<Algorithm>> algorithms;

	algorithms.push_back(std::make_unique<Apriori>());
	algorithms.push_back(std::make_unique<PCY>());
	algorithms.push_back(std::make_unique<Multihash>());


	std::vector<double> thresholds = { 0.01, 0.05, 0.10 };
	std::vector<double> datasetSizes = { 0.01, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 };

	for (auto& algorithm : algorithms) {
		std::cout << "Algorithm: " << abi::__cxa_demangle(typeid(*algorithm).name(), 0,0,0) << "\n";

		for (double threshold : thresholds) {
			for (double chunk : datasetSizes) {

				int basketCount = transactions.size() * chunk; //set run size
				//basketCount = std::clamp(basketCount, 0, static_cast<int>(transactions.size())); //Had some OOB segfaults so might need to clamp
				std::vector<std::vector<int>> dataset(transactions.begin(), transactions.begin() + basketCount); //enforce 
				int support = static_cast<int>(std::ceil(threshold * basketCount)); //convert support constants to specific number of baskets needed

				auto start = std::chrono::high_resolution_clock::now(); //might have some issues on different platforms, idk

				auto frequentPairs = algorithm->findFrequentPairs(dataset, support);

				auto end = std::chrono::high_resolution_clock::now();
				double elapsedTime = std::chrono::duration<double, std::milli>(end - start).count();


				// test
				std::cout << "Support: " <<	 threshold * 100 << "% Chunk: " << chunk * 100 << "% Frequent Pairs: " << frequentPairs.size() << " Elapsed time: " << elapsedTime << "ms\n";
			}
		}
		std::cout << "\n\n";
	}

	std::cin.get();
	return 0;
}