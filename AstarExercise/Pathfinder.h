#pragma once

#include "Pathfinding/BFS.h"
#include "Pathfinding/DFS.h"
#include "Pathfinding/Dijkstra.h"
#include "Pathfinding/Astar.h"

template <typename Graph, typename Location>
class Pathfinder {
public:
	Pathfinder(PathfindingStrategy<Graph, Location>* _strat) : strategy(_strat) {}

    bool MakeStep(Graph& graph, Location& goal) { return strategy->MakeStep(graph, goal, cameFrom, costSoFar, frontier); }

    void SwitchFuntion(PathfindingStrategy<Graph, Location>* _strat) { this->strategy = _strat; }
   
    void SetStart(Location& start) {
        this->start = start;
        frontier.put(start, 0);
        cameFrom[start] = start;
        costSoFar[start] = 0;
    }

    void Reset() {
        cameFrom.clear();

        costSoFar.clear();
        
        while (!frontier.isEmpty()) {
            frontier.get();
        }
    }

    std::vector<Location> PathTaken(Location& goal) { return ReconstructPath(start, goal, cameFrom); }

    const Location& GetStart() const { return start; }
    const std::unordered_map<Location, Location>& GetCameFrom() const { return cameFrom; }
    const std::unordered_map<Location, unsigned int>& GetCostSoFar() const { return costSoFar; }
    const unsigned int GetCostAtLocation(Location loc) { return costSoFar[loc]; }
    const PriorityQueue<Location, unsigned int>& GetFrontier() const { return frontier; }

private:
    PathfindingStrategy<Graph, Location>* strategy;
    
    Location start;
    std::unordered_map<Location, Location> cameFrom;
    std::unordered_map<Location, unsigned int> costSoFar;
    PriorityQueue<Location, unsigned int> frontier;
};
