#pragma once

#include <iostream>

#include <unordered_map>
#include <unordered_set>
#include <array>
#include <queue>
#include <set>

struct Square {
	int x, y;
	Square(int _x = 0, int _y = 0) : x(_x), y(_y) {}

	bool operator==(const Square& other) const {
		return this->x == other.x &&
			this->y == other.y;
	}

	bool operator !=(const Square& other)const {
		return !(*this == other);
	}

	bool operator<(const Square& other) const {
		return (this->x < other.x ||
			(this->x == other.x && this->y < other.y));
	}

	Square& operator=(const Square& other) {
		this->x = other.x;
		this->y = other.y;
		return *this;
	}
};

namespace std {
	// Implement hash function so we can put Square into an unordered_set
	template <> struct hash<Square> {
		std::size_t operator()(const Square& id) const noexcept {
			// NOTE: better to use something like boost hash_combine
			return std::hash<int>()(id.x ^ (id.y << 16));
		}
	};
}

class SquareGrid {
public:
	static const std::array<Square, 4> DIRS;
	
	SquareGrid(int _rows = 3, int _cols = 5) : rows(_rows), cols(_cols) { MakeGraph(); }

	void MakeGraph();

	void ReadGraph();

	std::vector<Square> Neighbors(Square& square);

	bool isInBounds(Square& square) {
		return (square.x >= 0 && square.x < cols&&
			square.y >= 0 && square.y < rows);
	}

private:
	int cols, rows;	//cols = width of the grid;		rows = height of the grid

	std::unordered_set<Square> nodes;	//switched to set as specyfing edges for each nodes is superflous
};

const std::array<Square, 4> SquareGrid::DIRS = {	/* East, West, North, South */
	Square{1, 0}, Square{-1, 0},
	Square{0, -1}, Square{0, 1}
};

void SquareGrid::MakeGraph() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Square current(i, j);	//Create current square

			this->nodes.insert(current);	//Add to graph
		}
	}
}

void SquareGrid::ReadGraph() {
	for (auto sq : nodes) {
		std::cout << "(" << sq.x << ", " << sq.y << ")" << std::endl;
	}
}

std::vector<Square> SquareGrid::Neighbors(Square& square) {
	std::vector<Square> neighbors;

	for (auto dir : DIRS) {
		Square next{ square.x + dir.x, square.y + dir.y };
		if (isInBounds(next)) {
			neighbors.push_back(next);
		}
	}

	if ((square.x + square.y) % 2 == 0) {
		std::reverse(neighbors.begin(), neighbors.end());
	}

	return neighbors;
}

/*void BreadthFirstSearch(SquareGrid& graph, Square& start) {
	std::queue<Square> frontier;
	frontier.push(start);

	std::set<Square> reached;
	reached.insert(start);

	while (!frontier.empty()) {
		Square current = frontier.front();
		frontier.pop();

		std::cout << "Visiting (" << current.x << ", " << current.y << ")" << std::endl;
		for (Square next : graph.neighbors(current)) {
			if (reached.find(next) == reached.end()) {
				frontier.push(next);
				reached.insert(next);
			}
		}
	}
}

std::unordered_map<Square, Square> BreadthFirstSearchToGoal(SquareGrid graph, Square& start, Square& goal) {
	std::queue<Square> frontier;
	frontier.push(start);

	std::unordered_map<Square, Square> cameFrom;
	cameFrom[start] = start;

	while (!frontier.empty()) {
		Square current = frontier.front();
		frontier.pop();

		if (current == goal) {
			break;
		}

		std::cout << "Visiting (" << current.x << ", " << current.y << ")" << std::endl;
		for (Square next : graph.neighbors(current)) {
			if (cameFrom.find(next) == cameFrom.end()) {
				frontier.push(next);
				cameFrom[next] = current;
			}
		}
	}

	return cameFrom;
}

std::vector<Square> ReconstructPath(const std::unordered_map<Square, Square>& path, Square& start, Square& goal) {
	std::vector<Square> reconstructedPath;

	Square current = goal;

	//Walk the path backwards 
	while (current != start) {
		reconstructedPath.push_back(current);
		current = path.at(current);
	}

	//Add the starting square
	reconstructedPath.push_back(start);

	//Reverse the path so that it goes from start to goal
	std::reverse(reconstructedPath.begin(), reconstructedPath.end());

	return reconstructedPath;
}
*/