#pragma once

#include "../Pathfinder/PathfindingUtilities.h"

template<typename Graph>
std::vector<SearchRecord<Graph>> DijkstraSearchRecorded(Graph& graph, typename Graph::location_t& start, typename Graph::location_t& goal, typename Graph::cost_t maxBudget = std::numeric_limits<typename Graph::cost_t>::max()) {
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
            for (auto it : costSoFar) {
                currentRecord.visited.push_back(std::make_pair(it.first, it.second));
            }

            auto tmp = frontier;    //frontier is a queue so it cannot be visited without destroying it
            while (!tmp.isEmpty()) {
                location_t it = tmp.get();
                currentRecord.discovered.push_back(std::make_pair(it, graph.Weight(it)));
            }

            currentRecord.pathToThisPoint = ReconstructPath(start, goal, cameFrom);
            break;
        }

        currentRecord.completed = false;

        for (auto it : costSoFar) {
            currentRecord.visited.push_back(std::make_pair(it.first, it.second));
        }

        for (location_t next : graph.Neighbors(current)) {
            cost_t newCost = costSoFar[current] + graph.Cost(current, next);

            if ( (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) &&
                 newCost <= maxBudget) {

                costSoFar[next] = newCost;
                cameFrom[next] = current;
                frontier.put(next, newCost);
            }
        }

        //currentRecord.toBeVisited = frontier
        auto tmp = frontier;    //frontier is a queue so it cannot be visited without destroying it
        while (!tmp.isEmpty()) {
            location_t it = tmp.get();
            currentRecord.discovered.push_back(std::make_pair(it, graph.Weight(it)));
        }

        result.push_back(currentRecord);
    }

    return result;
}

template<typename Graph>
class DijkstraStrategy : public PathfindingStrategy<Graph> {
public:
    DijkstraStrategy() {}

    const std::vector<SearchRecord<Graph>> MakeSearch(Graph& graph, typename Graph::location_t& start, typename Graph::location_t& goal, typename Graph::cost_t maxBudget) const override {
        return DijkstraSearchRecorded(graph, start, goal, maxBudget);
    }
};
