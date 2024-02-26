#pragma once
#include <fstream>
#include <functional> 
#include <unordered_map>
#include <random>

#include "../../Common/json.hpp"

#include "Square.h"

//Counter-clock wise
const std::array<Square, 4> SQUAREDIRS = {
	Square(0, -1),	//Up
	Square(-1, 0),	//Left
	Square(0, -1),	//Down
	Square(1, 0),	//Right
};

class SquareGrid  {
public:
	using location = Square;
	using cost_t = unsigned int;
	using tile = SquareTile;

	SquareGrid(const std::string& jsonFilePath = "BasicSquareMap.json");
	SquareGrid(unsigned	int _rows, unsigned int _cols, bool _weighted);

	void MakeGraph();
	void MakeRandomGraph();
	unsigned int RandomWeight();

	void ReadGrid();

	std::vector<Square> Neighbors(Square& square);

	void Increase(const Square& square) { nodes[square]++; }
	void Decrease(const Square& square) { (nodes[square] > 1) ? nodes[square]-- : nodes[square] == 1; }

	const unsigned int Rows() const { return rows; }
	const unsigned int Cols() const { return cols; }

	bool IsInBounds(Square& square) {
		if (square.x >= 0 && square.x < cols &&
			square.y >= 0 && square.y < rows) {
			return true;
		}
		return false;
	}

	unsigned int Weight(Square& square) {
		if (weighted) return nodes[square];
		else return 1;
	}
	unsigned int Cost(Square& start, Square& goal) {
		if (weighted) return nodes[start] + nodes[goal];
		else return 1;
	}

	inline unsigned int Heuristic(Square& start, Square& goal) {
		return (std::abs(start.x - goal.x) + std::abs(start.y - goal.y)) / 2;
	}

	const std::unordered_map<Square, unsigned int>& VisitNodes() const { return nodes; }

	nlohmann::json ToJson() const;

private:
	unsigned int rows, cols;

	bool weighted;

	std::unordered_map<Square, unsigned int> nodes;
};

SquareGrid::SquareGrid(const std::string& jsonFilePath) : rows(1), cols(1) {
	std::ifstream file(jsonFilePath);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << jsonFilePath << std::endl;
	}
	// Parse the JSON data
	nlohmann::json jsonData;
	try {
		file >> jsonData;

		rows = jsonData["rows"];
		cols = jsonData["cols"];

		MakeGraph();

		if (jsonData.find("weightedNodes") != jsonData.end()) {
			weighted = true;
			for (const auto& wSquare : jsonData["weightedNodes"]) {
				Square square{ wSquare["x"], wSquare["y"] };
				nodes[square] = wSquare["w"];
			}
		}
		else {
			weighted = false;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error parsing JSON: " << e.what() << std::endl;
	}
}

SquareGrid::SquareGrid(unsigned	int _rows, unsigned int _cols, bool _weighted) : rows(_rows), cols(_cols), weighted(_weighted ){
	if (weighted)
		MakeRandomGraph();
	else
		MakeGraph();
}

void SquareGrid::MakeGraph() {
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			Square current(i, j);

			this->nodes[current] = 1;
		}
	}
}

void SquareGrid::MakeRandomGraph() {
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			Square current(i, j);
			unsigned int w = RandomWeight();

			this->nodes[current] = w;
		}
	}
}

unsigned int SquareGrid::RandomWeight() {
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

void SquareGrid::ReadGrid() {
	std::cout << "Weighted Grid" << std::endl;
	for (const auto& it : nodes) {
		std::cout << it.first.PrintOut() << " | " << it.second << "\t";
	}
}

std::vector<Square> SquareGrid::Neighbors(Square& square) {
	std::vector<Square> neighbors;

	for (const auto& dir : SQUAREDIRS) {
		Square next(square + dir);
		if (IsInBounds(next)) {
			neighbors.push_back(next);
		}
	}

	return neighbors;
}

nlohmann::json SquareGrid::ToJson() const {
	nlohmann::json result;

	// Add radius and origin to JSON
	result["rows"] = rows;
	result["cols"] = cols;

	// Create an array for hexes with weight different from 1
	nlohmann::json squareArray = nlohmann::json::array();
	for (const auto& it : nodes) {
		if (it.second > 1) {
			nlohmann::json squareInfo = { {"x", it.first.x}, {"y", it.first.y}, {"w", it.second} };
			squareArray.push_back(squareInfo);
		}
	}

	// Add the hexArray to the result
	result["weightedNodes"] = squareArray;

	return result;
}