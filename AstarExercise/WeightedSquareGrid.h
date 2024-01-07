#pragma once
#include <iostream>
#include <random>

#include "SquareGrid.h"

inline double Heuristic(Square start, Square goal) {
	return (std::abs(start.x - goal.x) + std::abs(start.y - goal.y));
}

class WeightedSquareGrid : public SquareGrid {
public:
	WeightedSquareGrid(int _rows = 5, int _cols = 7) : SquareGrid(_rows, _cols) {
		std::cout << "WeightedSquareGrid" << std::endl;
		LoadForest();
	}

	double Cost(Square fromSquare, Square toSquare) const {
		return forests.find(toSquare) != forests.end() ? 5 : 1;
	}

private:
	void LoadForest();

	std::unordered_set<Square> forests;
};

void WeightedSquareGrid::LoadForest() {
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			// Seed the random number generator with a time-based seed
			std::random_device rd;
			std::mt19937 gen(rd());

			// Define the distribution for integers between 0 and 2
			std::uniform_int_distribution<int> distribution(0, 1);

			// Generate a random integer between 0 and 1
			int randomInt = distribution(gen);

			if (randomInt == 0) {
				forests.insert(Square(i, j));
			}
			std::cout << "Node (" << i << ", " << j << ") " << ((randomInt == 0) ? "is a forest" : "is NOT a forest") << std::endl;
		}
	}
}

template<typename item_t, typename priority_t>
struct PriorityQueue {
	typedef std::pair<priority_t, item_t> pairPI;

	std::priority_queue<pairPI, std::vector<pairPI>, std::greater<pairPI>> elements;

	inline bool isEmpty() { return elements.empty(); }

	inline void put(item_t item, priority_t priority) { elements.emplace(priority, item); }

	item_t get() {
		item_t best = elements.top().second;
		elements.pop();
		return best;
	}
};

template<typename Location, typename Graph>
void DijkstraSearch(Graph graph, Location start, Location goal, std::unordered_map<Location, Location>& cameFrom, std::unordered_map<Location, int>& costSoFar) {
	PriorityQueue<Location, int> frontier;
	frontier.put(start, 0);

	cameFrom[start] = start;
	costSoFar[start] = 0;

	while (!frontier.isEmpty()) {
		Location current = frontier.get();

		if (current == goal) {
			break;
		}

		for (Location next : graph.Neighbors(current)) {
			int newCost = costSoFar[current] + graph.cost(current, next);
			if (costSoFar.find(next) == costSoFar.end() ||
				newCost < costSoFar[next]) {
				costSoFar[next] = newCost;
				cameFrom[next] = current;
				frontier.put(next, newCost);
			}
		}
	}
}

template<typename Location>
std::vector<Location> ReconstructPath(Location start, Location goal, std::unordered_map<Location, Location> cameFrom) {
	std::vector<Location> path;
	Location current = goal;

	if (cameFrom.find(goal) == cameFrom.end()) {
		return path;
	}

	while (current != start) {
		path.push_back(current);
		current = cameFrom[current];
	}

	path.push_back(start);
	std::reverse(path.begin(), path.end());
	return path;
}

