#pragma once
#include "PathfindingUtilities.h"

template<typename Graph, typename Location>
unsigned int AstarSearch(Graph graph, Location start, Location goal, std::vector<Location>& pathTaken) {
	std::unordered_map<Location, Location> cameFrom;
	std::unordered_map<Location, unsigned int> costSoFar;

	PriorityQueue<Location, unsigned int> frontier;
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
	
	pathTaken = ReconstructPath(start, goal, cameFrom);
	return costSoFar[goal];
}

template<typename Graph, typename Location>
bool AstarSearchStep(Graph& graph, Location& goal, std::unordered_map<Location, Location>& cameFrom, std::unordered_map<Location, unsigned int>& costSoFar, PriorityQueue<Location, unsigned int>& frontier) {
    if (frontier.isEmpty()) {
        return true;
    }

    Location current = frontier.get();

    if (current == goal) {
        return true;
    }

    for (Location next : graph.Neighbors(current)) {
        unsigned int newCost = costSoFar[current] + graph.Cost(current, next);
		if (costSoFar.find(next) == costSoFar.end() ||
			newCost < costSoFar[next]) {
			costSoFar[next] = newCost;
			int priority = newCost + Heuristic(next, goal);
			frontier.put(next, priority);
			cameFrom[next] = current;
		}
    }

    return false;
}

template<typename Graph, typename Location>
void AstarSearchHelper(Graph& graph, Location& goal, std::unordered_map<Location, Location>& cameFrom, std::unordered_map<Location, unsigned int>& costSoFar, PriorityQueue<Location, unsigned int>& frontier) {
    if (!AstarSearchStep(graph, goal, cameFrom, costSoFar, frontier))
        AstarSearchHelper(graph, goal, cameFrom, costSoFar, frontier);
}

template<typename Graph, typename Location>
unsigned int AstarSearchRecursive(Graph& graph, Location& start, Location& goal, std::vector<Location>& pathTaken) {
    std::unordered_map<Location, Location> cameFrom;
    std::unordered_map<Location, unsigned int> costSoFar;

    PriorityQueue<Location, unsigned int> frontier;
    frontier.put(start, 0);

    cameFrom[start] = start;
    costSoFar[start] = 0;

    AstarSearchHelper(graph, goal, cameFrom, costSoFar, frontier);

    pathTaken = ReconstructPath(start, goal, cameFrom);
    return costSoFar[goal];
}

template<typename Graph, typename Location>
class AstarStrategy : public PathfindingStrategy<Graph, Location> {
public:
	AstarStrategy() {};

	//static AstarStrategy* CreateInstace(Graph& graph, Location& loc) override { return new AstarStrategy<Graph, Location> }

	bool MakeStep(Graph& graph, Location& goal,
		std::unordered_map<Location, Location>& cameFrom,
		std::unordered_map<Location, unsigned int>& costSoFar,
		PriorityQueue<Location, unsigned int>& frontier) override {
		return AstarSearchStep(graph, goal, cameFrom, costSoFar, frontier);
	}
};