#pragma once

#include <fstream>
#include "json.hpp"

#include "WeightedSquareGrid.h"
#include "WeightedHexagonalGrid.h"
#include "PathfindingAlgorithms.h"

int main() {
	std::cout << "Hello world!" << std::endl;
	
	/*Square example(1, 1);
	auto vec = graphExample.Neighbors(example);

	for (auto sq : vec) {
		std::cout << sq.x << "-" << sq.y << "\t";
	}
	
	SquareGrid graphExample;
	Square startExample(0, 0);
	std::cout << "Breadth first search from (0,0)" << std::endl;
	BreadthFirstSearch(graphExample, startExample);

	Square goalExample(1, 3);
	std::cout << "Breadth first search from (" << startExample.x << ", " << startExample.y
		<< ") to (" << goalExample.x << ", " << goalExample.y << ")" << std::endl;
	auto pathTaken = BreadthFirstSearchToGoal(graphExample, startExample, goalExample);

	auto reconstructed = ReconstructPath(pathTaken, startExample, goalExample);

	std::cout << "Edge walked: ";
	std::cout << "Path from (" << startExample.x << ", " << startExample.y << ") to (" << goalExample.x << ", " << goalExample.y << "):" << std::endl;
	for (const Square& square : reconstructed) {
		std::cout << "(" << square.x << ", " << square.y << ") ";
	}*/

 	/*WeightedSquareGrid example;

	Square start(0, 0);
	Square goal(6, 4);

	std::unordered_map<Square, Square> cameFrom;
	std::unordered_map<Square, int> costSoFar;

	AstarSearch(example, start, goal, cameFrom, costSoFar);

	std::cout << "CostSoFar " << costSoFar[goal];

	std::vector<Square> reconstructedPath = ReconstructPath(start, goal, cameFrom);
	for (Square sq : reconstructedPath) {
		std::cout << "( " << sq.x << ", " << sq.y << ")\t";
	}
	*/
	

	//Hex start(0, -2, 2);
	//Hex goal(-1, 1, 0);
	
	/*HexGrid hexGrid;
	
	//BreadthFirstSearch(hexGrid, start);
	
	auto path = BreadthFirstSearchToGoal(hexGrid, start, goal);
	auto reconstructedPath = ReconstructPath(path, start, goal);

	std::cout << "Path from:";
	start.Read();
	std::cout << " to:";
	goal.Read();
	std::cout << std::endl;
	for (const Hex& hex : reconstructedPath) {
		hex.Read();
	}*/

	/*WeightedHexGrid weightedHexGrid(2);

	std::unordered_map<Hex, Hex> cameFrom;
	std::unordered_map<Hex, int> costSoFar;

	std::cout << "Moving from ";
	start.Read();
	std::cout << " to ";
	goal.Read();
	std::cout << std::endl;

	AstarSearch(weightedHexGrid, start, goal, cameFrom, costSoFar);

	std::cout << "CostSoFar " << costSoFar[goal];

	std::vector<Hex> reconstructedPath = ReconstructPath(start, goal, cameFrom);
	for (const Hex& h : reconstructedPath) {
		h.Read();
	}*/

	WeightedHexGrid hexGraph(std::string("example.json"));

	//MakeGraphFromFile(std::string("example.json"));

	return 0;
}