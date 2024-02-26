#pragma once

#include "../Pathfinder/PathfindingUtilities.h"

template<typename Graph>
void BreadthFirstSearch(Graph& graph, typename Graph::location& start) {
	typedef typename Graph::location location;

	std::queue<location> frontier;
	frontier.push(start);

	std::set<location> reached;
	reached.insert(start);

	while (!frontier.empty()) {	//Continue the search until all locations are visited
		// Pop the current location from the front of the queue
		location current = frontier.front();
		frontier.pop();

		std::cout << current.PrintOut() << std::endl;	 // Visualize the current location

		// For each neighbor if it has not been reached before
		for (location next : graph.Neighbors(current)) {
			if (reached.find(next) == reached.end()) {
				// Add the neighbor to the queue and mark it as reached
				frontier.push(next);
				reached.insert(next);
			}
		}
	}
}

template<typename Graph>
bool BFSStep(Graph& graph, std::queue<typename Graph::location>& frontier, std::set<typename Graph::location>& reached) {
	typedef typename Graph::location location;
	
	if (frontier.empty()) {
		return true;
	}

	location current = frontier.front();
	frontier.pop();

	std::cout << "Lokking at: " << current.PrintOut() << std::endl;	 // Visualize the current location

	// For each neighbor if it has not been reached before
	for (location next : graph.Neighbors(current)) {
		if (reached.find(next) == reached.end()) {
			// Add the neighbor to the queue and mark it as reached
			frontier.push(next);
			reached.insert(next);
		}
	}

	return false;
}

template<typename Graph>
void BFSHelper(Graph& graph, std::queue<typename Graph::location >& frontier, std::set<typename Graph::location >& reached) {
	if (!BFSStep(graph, frontier, reached)) {
		BFSHelper(graph, frontier, reached);
	}
}


template<typename Graph>
void BFSRecursive(Graph& graph, typename Graph::location& start) {
	typedef typename Graph::location location;
	std::queue<location> frontier;
	frontier.push(start);

	std::set<location> reached;
	reached.insert(start);
	
	BFSHelper(graph, frontier, reached);
}

template<typename Graph>
std::unordered_map<typename Graph::location, typename Graph::location> BFSToGoal(Graph& graph, typename Graph::location& start, typename Graph::location& goal) {
	typedef typename Graph::location location;

	std::queue<location> frontier;
	frontier.push(start);

	std::unordered_map<location, location> cameFrom;
	cameFrom[start] = start;

	while (!frontier.empty()) {
		location current = frontier.front();
		frontier.pop();

		if (current == goal) {
			break;
		}

		for (location next : graph.Neighbors(current)) {
			if (cameFrom.find(next) == cameFrom.end()) {
				frontier.push(next);
				cameFrom[next] = current;
			}
		}
	}

	return cameFrom;
}

template<typename Graph>
bool BFSToGoalStep(Graph& graph, typename Graph::location& goal, std::set< typename Graph::location>& reached, std::queue< typename Graph::location>& frontier) {
	typedef typename Graph::location location;

	if (frontier.empty()) {
		std::cout << "Frontier is empty" << std::endl;
		return true;
	}

	location current = frontier.top();
	frontier.pop();
	std::cout << "Looking at: " << current.PrintOut() << "\t"; // Visualize the current location

	// Check if the current location is the goal
	if (current == goal) {
		std::cout << "Goal found: " << current.PrintOut() << std::endl;
		return true; // Exit the recursion if the goal is found
	}

	// For each neighbor if it has not been reached before
	for (location next : graph.Neighbors(current)) {
		if (location.find(next) == reached.end()) {
			std::cout << "Found not dicovered neighbor: " << next.PrintOut() << "\t";
			// Add the neighbor to the stack and mark it as reached
			frontier.push(next);
			reached.insert(next);
		}
	}

	return false;
}

template<typename Graph>
void BFSToGoalHelper(Graph& graph, typename Graph::location& goal, std::set<typename Graph::location >& reached, std::queue<typename Graph::location >& frontier) {
	if (!BFSToGoalStep(graph, goal, reached, frontier)) {
		BFSToGoalHelper(graph, goal, reached, frontier);
	}
}

template<typename Graph>
void BFSToGoalRecursive(Graph& graph, typename Graph::location& start, typename Graph::location& goal) {
	typedef typename Graph::location location;
	std::queue<location> frontier;
	frontier.push(start);

	std::set<location> reached;
	reached.insert(start);

	BFSToGoalHelper(graph, goal, reached, frontier);
}