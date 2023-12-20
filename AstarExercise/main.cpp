
#include <iostream>

#include <SFML/Graphics.hpp>

#include "SquareGrid.h"

int main() {
	std::cout << "Hello world!" << std::endl;
	
	SquareGrid graphExample;

	loadGraph(graphExample);

	std::cout << "First reading the graph:" << std::endl;
	readGraph(graphExample);

	//Square startExample(0, 0);
	/*std::cout << "Breadth first search from (0,0)" << std::endl;
	BreadthFirstSearch(example, startExample);*/

	/*Square goalExample(1, 3);
	std::cout << "Breadth first search from (" << startExample.x << ", " << startExample.y
		<< ") to (" << goalExample.x << ", " << goalExample.y << ")" << std::endl;
	auto pathTaken = BreadthFirstSearchToGoal(graphExample, startExample, goalExample);

	auto reconstructed = ReconstructPath(pathTaken, startExample, goalExample);

	std::cout << "Edge walked: ";
	std::cout << "Path from (" << startExample.x << ", " << startExample.y << ") to (" << goalExample.x << ", " << goalExample.y << "):" << std::endl;
	for (const Square& square : reconstructed) {
		std::cout << "(" << square.x << ", " << square.y << ") ";
	}*/

	return 0;
}