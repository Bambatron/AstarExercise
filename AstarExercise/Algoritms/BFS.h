#pragma once

#include "../Pathfinder/PathfindingUtilities.h"

template<typename Graph>
void BreadthFirstSearch(Graph& graph, typename Graph::location_t& start) {
	typedef typename Graph::location_t location_t;

	std::queue<location_t> frontier;
	frontier.push(start);

	std::set<location_t> reached;
	reached.insert(start);

	while (!frontier.empty()) {	//Continue the search until all locations are visited
		// Pop the current location from the front of the queue
		location_t current = frontier.front();
		frontier.pop();

		std::cout << current.PrintOut() << std::endl;	 // Visualize the current location

		// For each neighbor if it has not been reached before
		for (location_t next : graph.Neighbors(current)) {
			if (reached.find(next) == reached.end()) {
				// Add the neighbor to the queue and mark it as reached
				frontier.push(next);
				reached.insert(next);
			}
		}
	}
}

template<typename Graph>
bool BFSStep(Graph& graph, std::queue<typename Graph::location_t>& frontier, std::set<typename Graph::location_t>& reached) {
	typedef typename Graph::location_t location_t;
	
	if (frontier.empty()) {
		return true;
	}

	location_t current = frontier.front();
	frontier.pop();

	std::cout << "Lokking at: " << current.PrintOut() << std::endl;	 // Visualize the current location

	// For each neighbor if it has not been reached before
	for (location_t next : graph.Neighbors(current)) {
		if (reached.find(next) == reached.end()) {
			// Add the neighbor to the queue and mark it as reached
			frontier.push(next);
			reached.insert(next);
		}
	}

	return false;
}

template<typename Graph>
void BFSHelper(Graph& graph, std::queue<typename Graph::location_t >& frontier, std::set<typename Graph::location_t >& reached) {
	if (!BFSStep(graph, frontier, reached)) {
		BFSHelper(graph, frontier, reached);
	}
}


template<typename Graph>
void BFSRecursive(Graph& graph, typename Graph::location_t& start) {
	typedef typename Graph::location_t location_t;
	std::queue<location_t> frontier;
	frontier.push(start);

	std::set<location_t> reached;
	reached.insert(start);
	
	BFSHelper(graph, frontier, reached);
}

template<typename Graph>
std::unordered_map<typename Graph::location_t, typename Graph::location_t> BFSToGoal(Graph& graph, typename Graph::location_t& start, typename Graph::location_t& goal) {
	typedef typename Graph::location_t location_t;

	std::queue<location_t> frontier;
	frontier.push(start);

	std::unordered_map<location_t, location_t> cameFrom;
	cameFrom[start] = start;

	while (!frontier.empty()) {
		location_t current = frontier.front();
		frontier.pop();

		if (current == goal) {
			break;
		}

		for (location_t next : graph.Neighbors(current)) {
			if (cameFrom.find(next) == cameFrom.end()) {
				frontier.push(next);
				cameFrom[next] = current;
			}
		}
	}

	return cameFrom;
}

template<typename Graph>
bool BFSToGoalStep(Graph& graph, typename Graph::location_t& goal, std::set< typename Graph::location_t>& reached, std::queue< typename Graph::location_t>& frontier) {
	typedef typename Graph::location_t location_t;

	if (frontier.empty()) {
		std::cout << "Frontier is empty" << std::endl;
		return true;
	}

	location_t current = frontier.top();
	frontier.pop();
	std::cout << "Looking at: " << current.PrintOut() << "\t"; // Visualize the current location

	// Check if the current location is the goal
	if (current == goal) {
		std::cout << "Goal found: " << current.PrintOut() << std::endl;
		return true; // Exit the recursion if the goal is found
	}

	// For each neighbor if it has not been reached before
	for (location_t next : graph.Neighbors(current)) {
		if (location_t.find(next) == reached.end()) {
			std::cout << "Found not dicovered neighbor: " << next.PrintOut() << "\t";
			// Add the neighbor to the stack and mark it as reached
			frontier.push(next);
			reached.insert(next);
		}
	}

	return false;
}

template<typename Graph>
void BFSToGoalHelper(Graph& graph, typename Graph::location_t& goal, std::set<typename Graph::location_t >& reached, std::queue<typename Graph::location_t >& frontier) {
	if (!BFSToGoalStep(graph, goal, reached, frontier)) {
		BFSToGoalHelper(graph, goal, reached, frontier);
	}
}

template<typename Graph>
void BFSToGoalRecursive(Graph& graph, typename Graph::location_t& start, typename Graph::location_t& goal) {
	typedef typename Graph::location_t location_t;
	std::queue<location_t> frontier;
	frontier.push(start);

	std::set<location_t> reached;
	reached.insert(start);

	BFSToGoalHelper(graph, goal, reached, frontier);
}