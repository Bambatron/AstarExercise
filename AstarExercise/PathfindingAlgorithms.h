#pragma once

#include <queue>
#include <unordered_map>
#include <vector>

template<typename Graph, typename Location>
void BreadthFirstSearch(Graph& graph, Location& start) {
	std::queue<Location> frontier;
	frontier.push(start);

	std::set<Location> reached;
	reached.insert(start);

	int i = 0;

	while (!frontier.empty()) {
		Location current = frontier.front();
		frontier.pop();

		current.Read();
		for (Location next : graph.Neighbors(current)) {
			if (reached.find(next) == reached.end()) {
				frontier.push(next);
				reached.insert(next);
			}
		}
	}
}

template<typename Graph, typename Location>
std::unordered_map<Location, Location> BreadthFirstSearchToGoal(Graph& graph, Location& start, Location& goal) {
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

	//Add the starting square
	reconstructedPath.push_back(start);

	//Reverse the path so that it goes from start to goal
	std::reverse(reconstructedPath.begin(), reconstructedPath.end());

	return reconstructedPath;
}
