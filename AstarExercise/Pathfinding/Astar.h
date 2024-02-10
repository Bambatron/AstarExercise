#pragma once
#include "PathfindingUtilities.h"

template<typename Graph>
typename Graph::Cost_t AstarSearch(Graph& graph, typename Graph::Location& start, typename Graph::Location& goal,
	std::vector<typename Graph::Location>& pathTaken) {
	
	typedef typename Graph::Location Location;
	typedef typename Graph::Cost_t Cost_t;

	std::unordered_map<Location, Location> cameFrom;
	std::unordered_map<Location, Cost_t> costSoFar;

	PriorityQueue<Location, Cost_t> frontier;
	frontier.put(start, 0);

	cameFrom[start] = start;
	costSoFar[start] = 0;

	while (!frontier.isEmpty()) {
		Location current = frontier.get();

		if (current == goal) {
			break;
		}

		for (Location next : graph.Neighbors(current)) {
			Cost_t newCost = costSoFar[current] + graph.Cost(current, next);
			if (costSoFar.find(next) == costSoFar.end() ||
				newCost < costSoFar[next]) {
				costSoFar[next] = newCost;
				Cost_t priority = newCost + graph.Heuristic(next, goal);
				frontier.put(next, priority);
				cameFrom[next] = current;
			}
		}
	}
	
	pathTaken = ReconstructPath(start, goal, cameFrom);
	return costSoFar[goal];
}

template<typename Graph>
bool AstarSearchStep(Graph& graph, typename Graph::Location& goal,
	std::unordered_map<typename Graph::Location, typename Graph::Location>& cameFrom,
	std::unordered_map<typename Graph::Location, typename Graph::Cost_t>& costSoFar,
	PriorityQueue<typename Graph::Location, typename Graph::Cost_t>& frontier) {
	
	typedef typename Graph::Location Location;
	typedef typename Graph::Cost_t Cost_t;

	if (frontier.isEmpty()) {
        return true;
    }

    Location current = frontier.get();

    if (current == goal) {
        return true;
    }

    for (Location next : graph.Neighbors(current)) {
        Cost_t newCost = costSoFar[current] + graph.Cost(current, next);
		if (costSoFar.find(next) == costSoFar.end() ||
			newCost < costSoFar[next]) {
			costSoFar[next] = newCost;
			Cost_t priority = newCost + graph.Heuristic(next, goal);
			frontier.put(next, priority);
			cameFrom[next] = current;
		}
    }

    return false;
}

template<typename Graph>
void AstarSearchHelper(Graph& graph, typename Graph::Location& goal,
	std::unordered_map<typename Graph::Location, typename Graph::Location>& cameFrom, std::unordered_map<typename Graph::Location,
	typename Graph::Cost_t>& costSoFar,
	PriorityQueue<typename Graph::Location, typename Graph::Cost_t>& frontier) {

    if (!AstarSearchStep(graph, goal, cameFrom, costSoFar, frontier))
        AstarSearchHelper(graph, goal, cameFrom, costSoFar, frontier);
}

template<typename Graph>
typename Graph::Cost_t AstarSearchRecursive(Graph& graph, typename Graph::Location& start, typename Graph::Location& goal,
	std::function<typename Graph::Cost_t(typename Graph::Location a, typename Graph::Location b)> heuristic,
	std::vector<typename Graph::Location>& pathTaken) {
	
	typedef typename Graph::Location Location;
	typedef typename Graph::Cost_t Cost_t;

	std::unordered_map<Location, Location> cameFrom;
    std::unordered_map<Location, Cost_t> costSoFar;

    PriorityQueue<Location, Cost_t> frontier;
    frontier.put(start, 0);

    cameFrom[start] = start;
    costSoFar[start] = 0;

    AstarSearchHelper(graph, goal, cameFrom, costSoFar, frontier);

    pathTaken = ReconstructPath(start, goal, cameFrom);
    return costSoFar[goal];
}

template<typename Graph>
class AstarStrategy : public PathfindingStrategy<Graph> {
public:
	AstarStrategy() {};

	//static AstarStrategy* CreateInstace(Graph& graph, Location& loc) override { return new AstarStrategy<Graph, Location> }

	bool MakeStep(Graph& graph, typename Graph::Location& goal,
		std::unordered_map<typename Graph::Location, typename Graph::Location>& cameFrom,
		std::unordered_map<typename Graph::Location, typename Graph::Cost_t>& costSoFar,
		PriorityQueue<typename Graph::Location, typename Graph::Cost_t>& frontier) override {
		return AstarSearchStep(graph, goal, cameFrom, costSoFar, frontier);
	}
};