#pragma once

#include <fstream>
#include "json.hpp"

#include "WeightedSquareGrid.h"
#include "WeightedHexagonalGrid.h"
#include "PathfindingAlgorithms.h"

int main() {
	std::cout << "Hello world!" << std::endl;
	
	//Hex start(0, -2, 2);
	//Hex goal(-1, 1, 0);
	
	WeightedHexGrid hexGraph(std::string("example.json"));

	hexGraph.ReadGraph();

	return 0;
}