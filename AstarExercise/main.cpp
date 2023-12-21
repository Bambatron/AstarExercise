#include <iostream>

#include <SFML/Graphics.hpp>

#include "SquareGrid.h"

/*struct WeightedSquare : public Square {
	int w;
	WeightedSquare(int _x = 0, int _y = 0, int _w = 1) : Square(_x, _y), w(_w) {}

	bool operator==(const WeightedSquare& other) const {
		return this->x == other.x &&
			this->y == other.y &&
			this->w == other.w;
	}

	bool operator !=(const WeightedSquare& other)const {
		return !(*this == other);
	}

	bool operator<(const WeightedSquare& other) const {
		return (this->x < other.x ||
			(this->x == other.x && this->y < other.y));
	}

	WeightedSquare& operator=(const WeightedSquare& other) {
		this->x = other.x;
		this->y = other.y;
		this->w = other.w;
		return *this;
	}
};

struct WeightedSquareGrid : public SquareGrid {
	int rows, cols;
	WeightedSquareGrid(int _rows = 3, int _cols = 5) : SquareGrid(_rows, _cols) {}

	std::unordered_set<WeightedSquare> nodes;


};*/

int main() {
	std::cout << "Hello world!" << std::endl;
	
	SquareGrid graphExample;
	Square example(1, 1);
	auto vec = graphExample.Neighbors(example);

	for (auto sq : vec) {
		std::cout << sq.x << "-" << sq.y << "\t";
	}
	

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