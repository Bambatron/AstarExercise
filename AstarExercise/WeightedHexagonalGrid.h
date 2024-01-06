#pragma once
#include <random>

#include "HexagonalGrid.h"

inline double Heuristic(Hex start, Hex goal) {
	return (std::abs(start.q - goal.q) + std::abs(start.r - goal.r) + std::abs(start.s - goal.s)) / 2;
}

class WeightedHexGrid : public HexGrid {
public:
	WeightedHexGrid(int _radius = 4, const Hex& _origin = Hex(0, 0, 0)) : HexGrid(_radius, _origin) {
		MakeRandomForest();
	}

	WeightedHexGrid(const std::string& jsonFilePath);

	int Cost(Hex from, Hex to) const {
		return forests.find(to) != forests.end() ? 5 : 1;
	}

private:
	void MakeRandomForest();

	std::unordered_set<Hex> forests;

};

WeightedHexGrid::WeightedHexGrid(const std::string& jsonFilePath) : HexGrid(jsonFilePath) {
	std::cout << "WeightedHexGrid::WeightedHexGrid" << std::endl;
	// Read the JSON file
	std::ifstream file(jsonFilePath);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << jsonFilePath << std::endl;
	}

	// Parse the JSON data
	nlohmann::json jsonData;
	try {
		file >> jsonData;
		if (jsonData["forest"] == 1) {
			// Accessing values inside the "forests" array
			std::cout << "Forests:" << std::endl;
			for (const auto& forest : jsonData["forests"]) {
				int forest_q = forest["q"];
				int forest_r = forest["r"];
				std::cout << "  q = " << forest_q << ", r = " << forest_r << std::endl;
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error parsing JSON: " << e.what() << std::endl;
	}
}

void WeightedHexGrid::MakeRandomForest() {
	for (auto& hex : nodes) {
		// Seed the random number generator with a time-based seed
		std::random_device rd;
		std::mt19937 gen(rd());

		// Define the distribution for integers between 0 and 2
		std::uniform_int_distribution<int> distribution(0, 2);

		// Generate a random integer between 0 and 1
		int randomInt = distribution(gen);

		if (randomInt == 0) {
			forests.insert(Hex(hex));
		}
		std::cout << "Node";
		hex.Read();
		std::cout << ((randomInt == 0) ? "is a forest" : "is NOT a forest") << std::endl;
	}
}