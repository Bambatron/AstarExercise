#pragma once

#include "PathfindingUtilities.h"

template<typename Graph, typename Location>
void BreadthFirstSearch(Graph& graph, Location& start) {
	std::queue<Location> frontier;
	frontier.push(start);

	std::set<Location> reached;
	reached.insert(start);

	while (!frontier.empty()) {	//Continue the search until all locations are visited
		// Pop the current location from the front of the queue
		Location current = frontier.front();
		frontier.pop();

		std::cout << current.PrintOut() << std::endl;	 // Visualize the current location

		// For each neighbor if it has not been reached before
		for (Location next : graph.Neighbors(current)) {
			if (reached.find(next) == reached.end()) {
				// Add the neighbor to the queue and mark it as reached
				frontier.push(next);
				reached.insert(next);
			}
		}
	}
}

template<typename Graph, typename Location>
bool BFSStep(Graph& graph, std::queue<Location>& frontier, std::set<Location>& reached) {
	if (frontier.empty()) {
		return true;
	}

	Location current = frontier.front();
	frontier.pop();

	std::cout << "Lokking at: " << current.PrintOut() << std::endl;	 // Visualize the current location

	// For each neighbor if it has not been reached before
	for (Location next : graph.Neighbors(current)) {
		if (reached.find(next) == reached.end()) {
			// Add the neighbor to the queue and mark it as reached
			frontier.push(next);
			reached.insert(next);
		}
	}

	return false;
}

template<typename Graph, typename Location>
void BFSHelper(Graph& graph, std::queue<Location>& frontier, std::set<Location>& reached) {
	if (!BFSStep(graph, frontier, reached)) {
		BFSHelper(graph, frontier, reached);
	}
}


template<typename Graph, typename Location>
void BFSRecursive(Graph& graph, Location& start) {
	std::queue<Location> frontier;
	frontier.push(start);

	std::set<Location> reached;
	reached.insert(start);
	
	BFSHelper(graph, frontier, reached);
}

template<typename Graph, typename Location>
std::unordered_map<Location, Location> BFSToGoal(Graph& graph, Location& start, Location& goal) {
	std::queue<Location> frontier;
	frontier.push(start);

	std::unordered_map<Location, Location> cameFrom;
	cameFrom[start] = start;

	while (!frontier.empty()) {
		Location current = frontier.front();
		frontier.pop();

		if (current == goal) {
			break;
		}

		for (Location next : graph.Neighbors(current)) {
			if (cameFrom.find(next) == cameFrom.end()) {
				frontier.push(next);
				cameFrom[next] = current;
			}
		}
	}

	return cameFrom;
}

template<typename Graph, typename Location>
bool BFSToGoalStep(Graph& graph, Location& goal, std::set<Location>& reached, std::queue<Location>& frontier) {
	if (frontier.empty()) {
		std::cout << "Frontier is empty" << std::endl;
		return true;
	}

	Location current = frontier.top();
	frontier.pop();
	std::cout << "Looking at: " << current.PrintOut() << "\t"; // Visualize the current location

	// Check if the current location is the goal
	if (current == goal) {
		std::cout << "Goal found: " << current.PrintOut() << std::endl;
		return true; // Exit the recursion if the goal is found
	}

	// For each neighbor if it has not been reached before
	for (Location next : graph.Neighbors(current)) {
		if (reached.find(next) == reached.end()) {
			std::cout << "Found not dicovered neighbor: " << next.PrintOut() << "\t";
			// Add the neighbor to the stack and mark it as reached
			frontier.push(next);
			reached.insert(next);
		}
	}

	return false;
}

template<typename Graph, typename Location>
void BFSToGoalHelper(Graph& graph, Location& goal, std::set<Location>& reached, std::queue<Location>& frontier) {
	if (!BFSToGoalStep(graph, goal, reached, frontier)) {
		BFSToGoalHelper(graph, goal, reached, frontier);
	}
}

template<typename Graph, typename Location>
void BFSToGoalRecursive(Graph& graph, Location& start, Location& goal) {
	std::queue<Location> frontier;
	frontier.push(start);

	std::set<Location> reached;
	reached.insert(start);

	BFSToGoalHelper(graph, goal, reached, frontier);
}