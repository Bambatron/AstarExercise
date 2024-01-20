#pragma once

#include <queue>
#include <unordered_map>
#include <vector>

template<typename Graph, typename Location>
void BreadthFirstSearch(Graph& graph, Location& start, bool recursive = false) {
	std::queue<Location> frontier;
	frontier.push(start);

	std::set<Location> reached;
	reached.insert(start);

	if (!recursive) {
		while (!frontier.empty()) {	//Continue the search until all locations are visited
		// Pop the current location from the front of the queue
			Location current = frontier.front();
			frontier.pop();

			current.Read();	 // Visualize the current location

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
	else {
		RecursiveBFS(graph, frontier, reached);
	}
}

// Perform BFS recursively on the graph
template<typename Graph, typename Location>
void RecursiveBFS(Graph& graph, std::queue<Location>& frontier, std::set<Location>& reached) {
	
	if (frontier.empty()) {
		return;
	}

	// Dequeue front node and print it
	Location current = frontier.front();
	frontier.pop();
	
	current.Read();

	// do for every edge (v, u)
	for (Location next : graph.Neighbors(current)) {
		if (reached.find(next) == reached.end()) { // Mark neighbor as reached and enqueue it
			frontier.push(next);
			reached.insert(next);
		}
	}

	RecursiveBFS(graph, frontier, reached);
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
std::unordered_map<Location, Location> RecursiveBFSToGoal(Graph& graph, Location& goal, std::queue<Location>& frontier, std::unordered_map<Location, Location>& cameFrom) {
	std::cout << "RecursiveBFSToGoal" << std::endl;
	if (frontier.empty()) {
		// Goal not found
		return cameFrom;
	}

	Location current = frontier.front();
	frontier.pop();
	std::cout << "Current node: " << current.Read() << std::endl;

	// Check if the goal is reached
	if (current == goal) {
		// Goal found, stop the search
		std::cout << "Current is goal" << std::endl;
		return cameFrom;
	}

	// Explore neighbors
	for (Location& next : graph.Neighbors(current)) {
		if (cameFrom.find(next) == cameFrom.end()) {
			frontier.push(next);
			cameFrom[next] = current;
		}
	}

	// Continue the recursive BFS
	return RecursiveBFSToGoal(graph, goal, frontier, cameFrom);
}

template<typename Graph, typename Location>
std::unordered_map<Location, Location> StartRecursiveBFSToGoal(Graph& graph, Location& start, Location& goal) {
	std::cout << "StartRecursiveBFSToGoal from: " << start.Read() << " to: " << goal.Read() << std::endl;
	std::queue<Location> frontier;
	frontier.push(start);

	std::unordered_map<Location, Location> cameFrom;
	cameFrom[start] = start;

	return RecursiveBFSToGoal(graph, goal, frontier, cameFrom);
}

template<typename Graph, typename Location>
void AstarSearch(Graph graph, Location start, Location goal, std::unordered_map<Location, Location>& cameFrom, std::unordered_map<Location, int>& costSoFar) {
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
			int newCost = costSoFar[current] + graph.Cost(current, next);
			if (costSoFar.find(next) == costSoFar.end() ||
				newCost < costSoFar[next]) {
				costSoFar[next] = newCost;
				int priority = newCost + Heuristic(next, goal);
				frontier.put(next, priority);
				cameFrom[next] = current;
			}
		}
	}
}

template<typename Location>
std::vector<Location> ReconstructPath(const std::unordered_map<Location, Location>& path, Location& start, Location& goal) {
	std::vector<Location> reconstructedPath;

	Location current = goal;

	//Walk the path backwards 
	while (current != start) {
		reconstructedPath.push_back(current);
		current = path.at(current);
	}

	//Add the starting node
	reconstructedPath.push_back(start);

	//Reverse the path so that it goes from start to goal
	std::reverse(reconstructedPath.begin(), reconstructedPath.end());

	return reconstructedPath;
}

template<typename Graph, typename Location>
bool RecursiveAstarSearch(Graph graph, Location goal, PriorityQueue<Location, int> frontier, std::unordered_map<Location, Location>& cameFrom, std::unordered_map<Location, int>& costSoFar) {
	std::cout << "RecursiveAstarSearch" << std::endl;

	if (frontier.isEmpty()) {
		return false;
	}

	Location current = frontier.get();

	if (current == goal) {
		std::cout << "FOUND" << std::endl;
		return true;
	}

	for (Location next : graph.Neighbors(current)) {
		int newCost = costSoFar[current] + graph.Cost(current, next);
		if (costSoFar.find(next) == costSoFar.end() ||
			newCost < costSoFar[next]) {
			costSoFar[next] = newCost;
			int priority = newCost + Heuristic(next, goal);
			frontier.put(next, priority);
			cameFrom[next] = current;
		}
	}

	return RecursiveAstarSearch(graph, goal, frontier, cameFrom, costSoFar);
}

template<typename Graph, typename Location>
bool StartRecursiveAstarSearch(Graph graph, Location start, Location goal, std::unordered_map<Location, Location>& cameFrom, std::unordered_map<Location, int>& costSoFar) {
	std::cout << "StartRecursiveAstarSearch" << std::endl;
	PriorityQueue<Location, int> frontier;
	frontier.put(start, 0);

	cameFrom[start] = start;

	return RecursiveAstarSearch(graph, goal, frontier, cameFrom, costSoFar);
}