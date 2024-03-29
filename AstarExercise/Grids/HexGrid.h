#pragma once

#include <fstream>
#include <functional> 
#include <unordered_map>
#include <random>

#include "../../Common/json.hpp"

#include "Hex.h"
#include "Graph.h"

//Counter-clock wise
const std::array<Hex, 6> HEXDIRS = {
	Hex(1, 0, -1),	//Pointy: right		| Flat: down-right
	Hex(1, -1, 0),	//Pointy: up-right	| Flat: up-right
	Hex(0, -1, 1),	//Pointy: up-left	| Flat: up
	Hex(-1, 0, 1),	//Pointy: left		| Flat: up-left
	Hex(-1, 1, 0),	//Pointy: down-left	| Flat: down-left
	Hex(0, 1, -1)	//Pointy: down-right| Flat: down
};

class HexGrid : public Graph<Hex, unsigned int, HexTile>{
public:
	using location_t = Hex;
	using cost_t = unsigned int;
	using tile_t = HexTile;

	HexGrid(int _radius = 0, Hex _origin = Hex(0, 0), bool _weighted = false);

	void LoadFromString(const nlohmann::json& jsonObject);
	void LoadFromFile(const std::string& jsonFilePath = "BasicMapWeighted.json");

	void MakeGraph();
	void MakeRandomGraph();
	unsigned int RandomWeight();

	void ReadGrid();

	std::vector<Hex> Neighbors(const Hex& loc) const override;

	void Increase(const Hex& hex) { nodes[hex]++; }
	void Decrease(const Hex& hex) { (nodes[hex] > 1) ? nodes[hex]-- : nodes[hex] == 1; }

	const int Radius() const { return radius; }
	const Hex& Origin() const { return origin; }

	int Distance(const Hex& start, const Hex& goal) const {
		int dist = std::max(abs(start.q - goal.q), abs(start.r - goal.r));
		dist = std::max(dist, abs(start.s - goal.s));
		return dist;
	}

	bool IsInBounds(const Hex& loc) const override {
		if (Distance(origin, loc) <= radius) {
			return true;
		}
		return false;
	}

	unsigned int Weight(const Hex& loc) const override {
		if (weighted) return nodes[loc];
		else return 1;
	}
	unsigned int Cost(const Hex& start, const Hex& goal) const override {
		if (weighted) return nodes[start] + nodes[goal];
		else return 1;
	}

	unsigned int Heuristic(const Hex& start, const Hex& goal) const override {
		return (std::abs(start.q - goal.q) + std::abs(start.r - goal.r) + std::abs(start.s - goal.s)) / 2;
	}

	const std::unordered_map<Hex, unsigned int>& VisitNodes() const { return nodes; }

	nlohmann::json ToJson() const;

private:
	int radius;
	Hex origin;

	bool weighted;

	std::unordered_map<Hex, unsigned int> nodes;
};



HexGrid::HexGrid(int _radius, Hex _origin, bool _weighted) : Graph(_weighted), radius(_radius), origin(_origin) {
	if (weighted)
		MakeRandomGraph();
	else
		MakeGraph();
}

void HexGrid::LoadFromFile(const std::string& jsonFilePath) {
	radius = 0;
	origin = Hex{ 0, 0 };
	weighted = false;
	nodes.clear();

	std::ifstream file(jsonFilePath);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << jsonFilePath << std::endl;
	}
	// Parse the JSON data
	nlohmann::json jsonData;
	try {
		file >> jsonData;

		LoadFromString(jsonData);
	}
	catch (const std::exception& e) {
		std::cerr << "Error parsing JSON: " << e.what() << std::endl;
	}
}

void HexGrid::LoadFromString(const nlohmann::json& jsonObject) {
	radius = 0;
	origin = Hex{ 0, 0 };
	weighted = false;
	nodes.clear();

	int q = jsonObject["origin"]["q"];
	int r = jsonObject["origin"]["r"];
	this->origin = Hex(q, r);
	nodes[origin] = 1;

	this->radius = jsonObject["radius"];

	MakeGraph();

	if (jsonObject.find("weightedNodes") != jsonObject.end()) {
		weighted = true;
		for (const auto& wHex : jsonObject["weightedNodes"]) {
			Hex hex{ wHex["q"], wHex["r"] };
			nodes[hex] = wHex["w"];
		}
	}
	else {
		weighted = false;
	}
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

std::vector<Hex> HexGrid::Neighbors(const Hex& loc) const  {
	std::vector<Hex> neighbors;

	for (const auto& dir : HEXDIRS) {
		Hex next(loc + dir);
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
