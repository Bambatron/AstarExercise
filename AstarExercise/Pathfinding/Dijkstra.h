#pragma once

#include "PathfindingUtilities.h"

template< typename Graph, typename Location>
unsigned int DijkstraSearch(Graph& graph, Location& start, Location& goal, std::vector<Location>& pathTaken) {
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
            unsigned int newCost = costSoFar[current] + graph.Cost(current, next);
            if (costSoFar.find(next) == costSoFar.end()
                || newCost < costSoFar[next]) {
                costSoFar[next] = newCost;
                cameFrom[next] = current;
                frontier.put(next, newCost);
            }
        }
    }

    pathTaken = ReconstructPath(start, goal, cameFrom);
    return costSoFar[goal];
}

template<typename Graph, typename Location>
bool DijkstraSearchStep(Graph& graph, Location& goal, std::unordered_map<Location, Location>& cameFrom, std::unordered_map<Location, unsigned int>& costSoFar, PriorityQueue<Location, unsigned int>& frontier) {
    if (frontier.isEmpty()) {
        return true;
    }

    Location current = frontier.get();

    if (current == goal) {
        return true;
    }

    for (Location next : graph.Neighbors(current)) {
        unsigned int newCost = costSoFar[current] + graph.Cost(current, next);
        if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
            costSoFar[next] = newCost;
            cameFrom[next] = current;
            frontier.put(next, newCost);
        }
    }

    return false;
}

template<typename Graph, typename Location>
void DijkstraSearchHelper(Graph& graph, Location& goal, std::unordered_map<Location, Location>& cameFrom, std::unordered_map<Location, unsigned int>& costSoFar, PriorityQueue<Location, unsigned int>& frontier) {
    if(!DijkstraSearchStep(graph, goal, cameFrom, costSoFar, frontier)) 
        DijkstraSearchHelper(graph, goal, cameFrom, costSoFar, frontier);
}

template<typename Graph, typename Location>
unsigned int DijkstraSearchRecursive(Graph& graph, Location& start, Location& goal, std::vector<Location>& pathTaken) {
    std::unordered_map<Location, Location> cameFrom;
    std::unordered_map<Location, unsigned int> costSoFar;

    PriorityQueue<Location, unsigned int> frontier;
    frontier.put(start, 0);

    cameFrom[start] = start;
    costSoFar[start] = 0;

    DijkstraSearchHelper(graph, goal, cameFrom, costSoFar, frontier);

    pathTaken = ReconstructPath(start, goal, cameFrom);
    return costSoFar[goal];
}

template<typename Graph, typename Location>
class DijkstraStrategy : public PathfindingStrategy<Graph, Location> {
public:
    DijkstraStrategy() {}

    bool MakeStep(Graph& graph, Location& goal,
        std::unordered_map<Location, Location>& cameFrom,
        std::unordered_map<Location, unsigned int>& costSoFar,
        PriorityQueue<Location, unsigned int>& frontier) override {
        return DijkstraSearchStep(graph, goal, cameFrom, costSoFar, frontier);
    }
};