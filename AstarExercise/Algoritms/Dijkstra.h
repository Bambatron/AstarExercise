#pragma once

#include "../Pathfinder/PathfindingUtilities.h"

template< typename Graph>
typename Graph::Cost_t DijkstraSearch(Graph& graph, typename Graph::Location& start, typename Graph::Location& goal,
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

template<typename Graph>
bool DijkstraSearchStep(Graph& graph, typename Graph::Location& goal,
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
        if (costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]) {
            costSoFar[next] = newCost;
            cameFrom[next] = current;
            frontier.put(next, newCost);
        }
    }

    return false;
}

template<typename Graph>
void DijkstraSearchHelper(Graph& graph, typename Graph::Location& goal,
    std::unordered_map< typename Graph::Location, typename Graph::Location>& cameFrom,
    std::unordered_map< typename Graph::Location, typename Graph::Cost_t>& costSoFar,
    PriorityQueue< typename Graph::Location, typename Graph::Cost_t>& frontier) {
    if(!DijkstraSearchStep(graph, goal, cameFrom, costSoFar, frontier)) 
        DijkstraSearchHelper(graph, goal, cameFrom, costSoFar, frontier);
}

template<typename Graph>
unsigned int DijkstraSearchRecursive(Graph& graph, typename Graph::Location& start, typename Graph::Location& goal,
    std::vector<typename Graph::Location>& pathTaken) {
    
    typedef typename Graph::Location Location;
    typedef typename Graph::Cost_t Cost_t;
    
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

template<typename Graph>
class DijkstraStrategy : public PathfindingStrategy<Graph> {
public:
    DijkstraStrategy() {}
   // static  DijkstraStrategy* CreateInstace(Graph& graph, Location& loc) override { return new DijkstraStrategy<Graph, Location>}

    bool MakeStep(Graph& graph, typename Graph::Location& goal,
        std::unordered_map<typename Graph::Location, typename Graph::Location>& cameFrom,
        std::unordered_map<typename Graph::Location, typename Graph::Cost_t>& costSoFar,
        PriorityQueue<typename Graph::Location, typename Graph::Cost_t>& frontier) override {
        return DijkstraSearchStep(graph, goal, cameFrom, costSoFar, frontier);
    }
};