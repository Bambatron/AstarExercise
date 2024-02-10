#pragma once

#include <fstream>
#include <functional> 
#include <unordered_map>
#include <random>

#include "json.hpp"

#include "Hex.h"

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

class HexGrid {
public:
	using Location = Hex;
	using Cost_t = unsigned int;

	HexGrid(const std::string& jsonFilePath = "BasicMapWeighted.json");
	HexGrid(int _radius, Hex _origin);

	void MakeGraph();
	void MakeRandomGraph();
	Cost_t RandomWeight();

	void ReadGrid();

	std::vector<Location > Neighbors(Location& hex);

	void Increase(const Location& hex) { nodes[hex]++; }
	void Decrease(const Location& hex) { (nodes[hex] > 1) ? nodes[hex]-- : nodes[hex] == 1; }

	const int Radius() const { return radius; }
	const Location& Origin() const { return origin; }

	int Distance(Location& start, Location& goal) {
		int dist = std::max(abs(start.q - goal.q), abs(start.r - goal.r));
		dist = std::max(dist, abs(start.s - goal.s));
		return dist;
	}

	bool IsInBounds(Location& hex) {
		if (Distance(origin, hex) <= radius) {
			return true;
		}
		return false;
	}

	Cost_t Weight(Location& hex) {
		if (_weighted) return nodes[hex];
		else return 1;
	}
	Cost_t Cost(Location& start, Location& goal) {
		if (_weighted) return nodes[start] + nodes[goal];
		else return 1;
	}

	inline Cost_t Heuristic(Location  start, Location  goal) {
		return (std::abs(start.q - goal.q) + std::abs(start.r - goal.r) + std::abs(start.s - goal.s)) / 2;
	}


	const std::unordered_map<Location, Cost_t>& VisitNodes() const { return nodes; }

	nlohmann::json ToJson() const;

private:
	int radius;
	Location origin;

	bool _weighted;

	std::unordered_map<Location, Cost_t> nodes;
};

HexGrid::HexGrid(const std::string& jsonFilePath) : radius(0), origin(Hex(0, 0)) {
	std::ifstream file(jsonFilePath);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << jsonFilePath << std::endl;
	}
	// Parse the JSON data
	nlohmann::json jsonData;
	try {
		file >> jsonData;

		int q = jsonData["origin"]["q"];
		int r = jsonData["origin"]["r"];
		this->origin = Hex(q, r);
		nodes[origin] = 1;

		this->radius = jsonData["radius"];

		MakeGraph();

		if (jsonData.find("weightedNodes") != jsonData.end()) {
			_weighted = true;
			for (const auto& wHex : jsonData["weightedNodes"]) {
				Hex hex{ wHex["q"], wHex["r"] };
				nodes[hex] = wHex["w"];
			}
		}
		else {
			_weighted = false;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error parsing JSON: " << e.what() << std::endl;
	}
}

HexGrid::HexGrid(int _radius, Hex _origin) : radius(_radius), origin(_origin) {
	MakeRandomGraph();
}

void HexGrid::MakeGraph() {
	for (int q = -radius; q <= radius; q++) {
		int start = std::max(-radius, -q - radius);
		int end = std::min(radius, radius - q);
		for (int r = start; r <= end; r++) {
			Hex current(q, r);

			this->nodes[current + this->origin] = 1;
		}
	}
}

void HexGrid::MakeRandomGraph() {
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

unsigned int HexGrid::RandomWeight() {
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

void HexGrid::ReadGrid() {
	std::cout << "Weighted Grid" << std::endl;
	for (const auto& it : nodes) {
		std::cout << it.first.PrintOut() << " | " << it.second << "\t";
	}
}

std::vector<Hex> HexGrid::Neighbors(Hex& hex) {
	std::vector<Hex> neighbors;

	for (const auto& dir : DIRS) {
		Hex next(hex + dir);
		if (IsInBounds(next)) {
			neighbors.push_back(next);
		}
	}

	return neighbors;
}

nlohmann::json HexGrid::ToJson() const {
	nlohmann::json result;

	// Add radius and origin to JSON
	result["radius"] = radius;
	result["origin"] = { {"q", origin.q}, {"r", origin.r} };

	// Create an array for hexes with weight different from 1
	nlohmann::json hexArray = nlohmann::json::array();
	for (const auto& it : nodes) {
		if (it.second > 1) {
			nlohmann::json hexInfo = { {"q", it.first.q}, {"r", it.first.r}, {"w", it.second} };
			hexArray.push_back(hexInfo);
		}
	}

	// Add the hexArray to the result
	result["weightedNodes"] = hexArray;

	return result;
}
