#pragma once

#include "Pathfinding/BFS.h"
#include "Pathfinding/DFS.h"
#include "Pathfinding/Dijkstra.h"
#include "Pathfinding/Astar.h"

template <typename Graph, typename Location>
class Pathfinder {
public:
	Pathfinder(PathfindingStrategy<Graph, Location>* _strat) : strategy(_strat) {
        _startSelected = false;
    }

    bool MakeStep(Graph& graph) { return strategy->MakeStep(graph, goal, cameFrom, costSoFar, frontier); }

    void SwitchFuntion(PathfindingStrategy<Graph, Location>* _strat) { this->strategy = _strat; }

    void SetStart(Location start) {
        if (!_startSelected) {
            _startSelected = true;
            this->start = start;
            frontier.put(start, 0);
            cameFrom[start] = start;
            costSoFar[start] = 0;
        }
    }
    void ClearStart() { _startSelected = false; }
    void SetGoal(Location goal) {
        if (!_goalSelected) {
            _goalSelected = true;
            this->goal = goal;
        }
    }
    void ClearGoal() { _goalSelected = false; }

    void Reset() {
        _startSelected = false;
        _goalSelected = false;
        cameFrom.clear();
        costSoFar.clear();
        while (!frontier.isEmpty()) {
            frontier.get();
        }
    }

    std::vector<Location> PathTaken(Location& goal) { return ReconstructPath(start, goal, cameFrom); }

    bool IsStartSelected() const { return _startSelected; }
    bool IsStartSelected(Location& loc) const {
        if (_startSelected)
            loc = start;
        return _startSelected;
    }
    const Location& GetStart() const { return start; }
    bool IsGoalSelected() const { return _goalSelected; }
    bool IsGoalSelected(Location& loc) const {
        if (_goalSelected)
            loc = goal;
        return _goalSelected;
    }
    const Location& GetGoal() const { return goal; }
    
    const std::unordered_map<Location, Location>& GetCameFrom() const { return cameFrom; }
    const std::unordered_map<Location, unsigned int>& GetCostSoFar() const { return costSoFar; }
    const unsigned int GetCostAtLocation(Location loc) { return costSoFar[loc]; }
    const PriorityQueue<Location, unsigned int>& GetFrontier() const { return frontier; }

private:
    PathfindingStrategy<Graph, Location>* strategy;

    Location start;
    bool _startSelected;
    Location goal;
    bool _goalSelected;
    
    std::unordered_map<Location, Location> cameFrom;
    std::unordered_map<Location, unsigned int> costSoFar;
    PriorityQueue<Location, unsigned int> frontier;
};
