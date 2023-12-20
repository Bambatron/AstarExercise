#pragma once

#include <iostream>

#include <unordered_map>
#include <queue>
#include <set>

/// <summary>
/// The Node used by the Graph
/// </summary>
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

/// <summary>
/// The possibles directions in a square grid
/// </summary>
const std::vector<Square> directions = {
	Square(-1, 0),	//UP
	Square(0, 1),	//RIGHT
	Square(1, 0),	//DOWN
	Square(0, -1)	//LEFT
};

/// <summary>
/// Use XOR between the x coordinate and the y coordinated left-shifted by 16 
/// </summary>
class SquaresHashFunction {
public:
	size_t operator()(const Square& p) const {
		return std::hash<int>()(p.x ^ (p.y << 16));
	}
};


/// <summary>
/// The Graph
/// </summary>
struct SquareGrid {
	int cols, rows;	//cols = width of the grid;		rows = height of the grid
	SquareGrid(int _rows = 3, int _cols = 5) : rows(_rows), cols(_cols) {}

	std::unordered_map<Square, std::vector<Square>, SquaresHashFunction> edges;

	/// <summary>
	/// Return the nodes near the provided node
	/// </summary>
	/// <param name="sq">The node you want the neighbor of</param>
	/// <returns></returns>
	std::vector<Square> neighbors(Square sq) {
		return edges[sq];
	}
};



/// <summary>
/// Creates the nodes and edges of the graph
/// </summary>
/// <param name="graph">The graph to load</param>
void loadGraph(SquareGrid& graph) {
	int rows = graph.rows;
	int cols = graph.cols;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			//Create current sqaure
			Square current(i, j);

			//Search for neighbors
			std::vector<Square> neighbors;
			for (const Square& dir : directions) {
				if (i + dir.x >= 0 && i + dir.x < rows &&
					j + dir.y >= 0 && j + dir.y < cols) {
					Square n(i + dir.x, j + dir.y);

					neighbors.push_back(n);
				}
			}

			//Add to graph
			graph.edges.insert({ current, neighbors });
		}

	}
}

/// <summary>
/// Prints out each node and its neighbors
/// </summary>
/// <param name="graph">The graph to read</param>
void readGraph(SquareGrid graph) {
	for (auto sq = graph.edges.begin(); sq != graph.edges.end(); sq++) {
		std::cout << "Visiting (" << sq->first.x << ", " << sq->first.y << ") with neighbors:\t";
		for (auto neighbor : sq->second) {
			std::cout << "(" << neighbor.x << ", " << neighbor.y << ")\t";
		}
		std::cout << std::endl;
	}
}

/// <summary>
/// Simple breadth first search on the given graph from the given node
/// </summary>
/// <param name="graph">The graph to execute the search on</param>
/// <param name="start">The node from which to start the search</param>
void BreadthFirstSearch(SquareGrid graph, Square& start) {
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

namespace std {
	template <>
	struct hash<Square> {
		size_t operator()(const Square& s) const {
			// Define a hash function for Square
			return hash<int>()(s.x) ^ (hash<int>()(s.y) << 1);
		}
	};
}

/// <summary>
/// Simple breadth first search on the given graph from the given node with an early exit on the given node
/// </summary>
/// <param name="graph">The graph to execute the search on</param>
/// <param name="start">The node from which to start the search</param>
/// <param name="goal">The node on which to end early the search</param>
/// <returns></returns>
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

/// <summary>
/// Re-orders the path created by a search
/// </summary>
/// <param name="path">The path created by a search</param>
/// <param name="start">The node where the search starts</param>
/// <param name="goal">The node where the search ends</param>
/// <returns>The ordered path from the start node to the goal node</returns>
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