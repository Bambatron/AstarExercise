#pragma once

#include "../Pathfinder/PathfindingUtilities.h"

template<typename Graph>
std::vector<SearchRecord<Graph>> AstarSearchRecorded(Graph& graph, typename Graph::location_t& start, typename Graph::location_t& goal, std::vector<typename Graph::location_t>& pathTaken) {
    typedef typename Graph::location_t location_t;
    typedef typename Graph::cost_t cost_t;

    std::vector<SearchRecord<Graph>> result;

    std::unordered_map<location_t, location_t> cameFrom;
    std::unordered_map<location_t, cost_t> costSoFar;

    PriorityQueue<location_t, cost_t> frontier;

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
		location_t current = frontier.get();

        SearchRecord<Graph> currentRecord;
        currentRecord.currentNode = current;

        if (current == goal) {
            currentRecord.completed = true;
            break;
        }

        currentRecord.completed = false;

        for (auto it : costSoFar) {
            currentRecord.visited.push_back(std::make_pair(it.first, it.second));
        }

		for (location_t next : graph.Neighbors(current)) {
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
			location_t it = tmp.get();
			std::pair<location_t, cost_t> nPair = std::make_pair(it, graph.Weight(it));
            currentRecord.discovered.push_back(nPair);
        }

		result.push_back(currentRecord);
    }

    pathTaken = ReconstructPath(start, goal, cameFrom);

    return result;
}

template<typename Graph>
class AstarStrategy : public PathfindingStrategy<Graph> {
public:
	AstarStrategy() {};

	std::vector<SearchRecord<Graph>> MakeSearch(Graph& graph, typename Graph::location_t& start, typename Graph::location_t& goal, std::vector<typename Graph::location_t>& pathTaken) override {
		return AstarSearchRecorded(graph, start, goal, pathTaken);
	}
};
