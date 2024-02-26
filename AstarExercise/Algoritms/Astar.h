#pragma once

#include "../Pathfinder/PathfindingUtilities.h"

template<typename Graph>
std::vector<SearchRecord<Graph>> AstarSearchRecorded(Graph& graph, typename Graph::location& start, typename Graph::location& goal, std::vector<typename Graph::location>& pathTaken) {
    typedef typename Graph::location location;
    typedef typename Graph::cost_t cost_t;

    std::vector<SearchRecord<Graph>> result;

    std::unordered_map<location, location> cameFrom;
    std::unordered_map<location, cost_t> costSoFar;

    PriorityQueue<location, cost_t> frontier;

    if (!graph.IsInBounds(start)) {
        throw std::runtime_error("Search Failed: Start node is not in the graph");
    }
    if (!graph.IsInBounds(goal)) {
        throw std::runtime_error("Search failed: Goal node is not in the graph");
    }

    frontier.put(start, 0);

    cameFrom[start] = start;
    costSoFar[start] = 0;

    while (!frontier.isEmpty()) {
        location current = frontier.get();

        SearchRecord<Graph> currentRecord;
        currentRecord.currentNode = current;

        if (current == goal) {
            currentRecord.completed = true;
            break;
        }

        currentRecord.completed = false;

        for (auto it : costSoFar) {
            currentRecord.alreadyVisited.push_back(std::make_pair(it.first, it.second));
        }

		for (location next : graph.Neighbors(current)) {
			cost_t newCost = costSoFar[current] + graph.Cost(current, next);
			if (costSoFar.find(next) == costSoFar.end() ||
				newCost < costSoFar[next]) {
				costSoFar[next] = newCost;
				cost_t priority = newCost + graph.Heuristic(next, goal);
				frontier.put(next, priority);
				cameFrom[next] = current;
			}
		}

        //currentRecord.toBeVisited = frontier
        auto tmp = frontier;    //frontier is a queue so it cannot be visited without destroying it
        while (!tmp.isEmpty()) {
			location it = tmp.get();
			std::pair<location, cost_t> nPair = std::make_pair(it, graph.Weight(it));
            currentRecord.toBeVisited.push_back(nPair);
        }

		result.push_back(currentRecord);
    }

    pathTaken = ReconstructPath(start, goal, cameFrom);

    return result;
}

template<typename Graph>
typename Graph::cost_t AstarSearch(Graph& graph, typename Graph::location& start, typename Graph::location& goal,
	std::vector<typename Graph::location>& pathTaken) {
	
	typedef typename Graph::location location;
	typedef typename Graph::cost_t cost_t;

	std::unordered_map<location, location> cameFrom;
	std::unordered_map<location, cost_t> costSoFar;

	PriorityQueue<location, cost_t> frontier;
	frontier.put(start, 0);

	cameFrom[start] = start;
	costSoFar[start] = 0;

	while (!frontier.isEmpty()) {
		location current = frontier.get();

		if (current == goal) {
			break;
		}

		for (location next : graph.Neighbors(current)) {
			cost_t newCost = costSoFar[current] + graph.Cost(current, next);
			if (costSoFar.find(next) == costSoFar.end() ||
				newCost < costSoFar[next]) {
				costSoFar[next] = newCost;
				cost_t priority = newCost + graph.Heuristic(next, goal);
				frontier.put(next, priority);
				cameFrom[next] = current;
			}
		}
	}
	
	pathTaken = ReconstructPath(start, goal, cameFrom);
	return costSoFar[goal];
}

template<typename Graph>
bool AstarSearchStep(Graph& graph, typename Graph::location& goal,
	std::unordered_map<typename Graph::location, typename Graph::location>& cameFrom,
	std::unordered_map<typename Graph::location, typename Graph::cost_t>& costSoFar,
	PriorityQueue<typename Graph::location, typename Graph::cost_t>& frontier) {
	
	typedef typename Graph::location location;
	typedef typename Graph::cost_t cost_t;

	if (frontier.isEmpty()) {
        return true;
    }

	location current = frontier.get();

    if (current == goal) {
        return true;
    }

    for (location next : graph.Neighbors(current)) {
		cost_t newCost = costSoFar[current] + graph.Cost(current, next);
		if (costSoFar.find(next) == costSoFar.end() ||
			newCost < costSoFar[next]) {
			costSoFar[next] = newCost;
			cost_t priority = newCost + graph.Heuristic(next, goal);
			frontier.put(next, priority);
			cameFrom[next] = current;
		}
    }

    return false;
}

template<typename Graph>
void AstarSearchHelper(Graph& graph, typename Graph::location& goal,
	std::unordered_map<typename Graph::location, typename Graph::location>& cameFrom,
	std::unordered_map<typename Graph::location, typename Graph::cost_t>& costSoFar,
	PriorityQueue<typename Graph::location, typename Graph::cost_t>& frontier) {

    if (!AstarSearchStep(graph, goal, cameFrom, costSoFar, frontier))
        AstarSearchHelper(graph, goal, cameFrom, costSoFar, frontier);
}

template<typename Graph>
typename Graph::cost_t AstarSearchRecursive(Graph& graph, typename Graph::location& start, typename Graph::location& goal,
	std::function<typename Graph::cost_t(typename Graph::location a, typename Graph::location b)> heuristic,
	std::vector<typename Graph::location>& pathTaken) {
	
	typedef typename Graph::location location;
	typedef typename Graph::cost_t cost_t;

	std::unordered_map<location, location> cameFrom;
    std::unordered_map<location, cost_t> costSoFar;

    PriorityQueue<location, cost_t> frontier;
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

	std::vector<SearchRecord<Graph>> MakeSearch(Graph& graph, typename Graph::location& start, typename Graph::location& goal, std::vector<typename Graph::location>& pathTaken) override {
		return AstarSearchRecorded(graph, start, goal, pathTaken);
	}

	bool MakeStep(Graph& graph, typename Graph::location& goal,
		std::unordered_map<typename Graph::location, typename Graph::location>& cameFrom,
		std::unordered_map<typename Graph::location, typename Graph::cost_t>& costSoFar,
		PriorityQueue<typename Graph::location, typename Graph::cost_t>& frontier) override {
		return AstarSearchStep(graph, goal, cameFrom, costSoFar, frontier);
	}
};
