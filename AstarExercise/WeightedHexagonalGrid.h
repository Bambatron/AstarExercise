#pragma once
#include <random>

#include "HexagonalGrid.h"

namespace std {
	template <> struct hash<std::pair<Hex, unsigned int>> {
		size_t operator()(const std::pair<Hex, unsigned int>& wh) const {
			hash<int> int_hash;
			size_t hq = int_hash(wh.first.q);
			size_t hr = int_hash(wh.first.r);
			return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
		}
	};
}

class WeightedHexGrid {
public:
	WeightedHexGrid(const std::string& jsonFilePath = "BasicMapWeighhted.json");
	WeightedHexGrid(int _radius, Hex _origin);

	void MakeGraph();
	void MakeRandomGraph();

	unsigned int RandomWeight();

	void ReadGrid();

	void Increase(const Hex& hex) { nodes[hex]++; }
	void Decrease(const Hex& hex) { (nodes[hex] > 1) ? nodes[hex]-- : nodes[hex]==1; }

	const std::unordered_map<Hex, unsigned int>& VisitNodes() const { return nodes; }

private:
	int radius;
	Hex origin;

	std::unordered_map<Hex, unsigned int> nodes;
};

WeightedHexGrid::WeightedHexGrid(const std::string& jsonFilePath) : radius(0), origin(Hex(0,0)) {
	std::ifstream file(jsonFilePath);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << jsonFilePath << std::endl;
	}
	// Parse the JSON data
	nlohmann::json jsonData;
	try {
		file >> jsonData;
		// Accessing individual values
		this->radius = jsonData["radius"];

		// Accessing values inside the "origin" object
		int q = jsonData["origin"]["q"];
		int r = jsonData["origin"]["r"];
		unsigned int w = jsonData["origin"]["w"];
		this->origin = Hex(q, r);
		nodes[origin] = w;

		MakeGraph();

		jsonData["forests"];
	}
	catch (const std::exception& e) {
		std::cerr << "Error parsing JSON: " << e.what() << std::endl;
	}

}

WeightedHexGrid::WeightedHexGrid(int _radius, Hex _origin) : radius(_radius), origin(_origin) {
	MakeRandomGraph();
}

void WeightedHexGrid::MakeGraph() {
	for (int q = -radius; q <= radius; q++) {
		int start = std::max(-radius, -q - radius);
		int end = std::min(radius, radius - q);
		for (int r = start; r <= end; r++) {
			Hex current(q, r);

			this->nodes[current + this->origin] =  1;
		}
	}
}

void WeightedHexGrid::MakeRandomGraph() {
	this->nodes.insert(std::pair<Hex, unsigned int>(origin, RandomWeight()));
	
	for (int q = -radius; q <= radius; q++) {
		int start = std::max(-radius, -q - radius);
		int end = std::min(radius, radius - q);
		for (int r = start; r <= end; r++) {
			Hex current(q, r);
			unsigned int w = RandomWeight();
			this->nodes[current + this->origin] = w;
		}
	}
}

unsigned int WeightedHexGrid::RandomWeight() {
	unsigned int ret = 0;
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define the distribution for integers between 0 and 99
	std::uniform_int_distribution<int> distribution(0, 99);

	// Generate a random integer between 0 and 1
	int randomInt = distribution(gen);

	if (randomInt < 27) {
		ret = 1;
	}
	else if (randomInt >= 27 && randomInt < 47) {
		ret = 2;
	}
	else if (randomInt >= 47 && randomInt < 62) {
		ret = 3;
	}
	else if (randomInt >= 62 && randomInt < 72) {
		ret = 4;
	}
	else if (randomInt >= 72 && randomInt < 82) {
		ret = 5;
	}
	else if (randomInt >= 82 && randomInt < 89) {
		ret = 6;
	}
	else if (randomInt >= 89 && randomInt < 94) {
		ret = 7;
	}
	else if (randomInt >= 94 && randomInt < 97) {
		ret = 8;
	}
	else if (randomInt >= 97 && randomInt < 99) {
		ret = 9;
	}
	else if (randomInt == 99) {
		ret = 10;
	}

	return ret;
}

void WeightedHexGrid::ReadGrid() {
	std::cout << "Weighted Grid" << std::endl;
	for (const auto& it : nodes) {
		std::cout << it.first.Read() << " | " << it.second << "\t";
	}
}