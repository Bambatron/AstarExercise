#pragma once

#include "Pathfinding/BFS.h"
#include "Pathfinding/DFS.h"
#include "Pathfinding/Dijkstra.h"
#include "Pathfinding/Astar.h"

template<typename Location>
struct SearchRecord {
    bool completed;
    Location currentNode;
    std::vector<std::pair<Location, Location>> cameFrom;    //Nodes of the path to go to the search goal
    std::vector<std::pair<Location, unsigned int>> costSoFar;   //Cost to go to each location
    std::vector<Location> frontier;    //Discovered noedds
};

template <typename Graph>
class Pathfinder {
public:
    using Location = typedef typename Graph::Location;
    using Cost_t = typedef typename Graph::Cost_t;

	Pathfinder(PathfindingStrategy<Graph>* _strat) : strategy(_strat) {
        _startSelected = false;
        _goalSelected = false;
    }

    bool MakeStep(Graph& graph);

    void SwitchFuntion(PathfindingStrategy<Graph>* _strat) { this->strategy = _strat; }

   void Reset() {
       _startSelected = false;
       _goalSelected = false;
       cameFrom.clear();
       costSoFar.clear();
       while (!frontier.isEmpty()) {
           frontier.get();
       }
       currentRecord = 0;
       record.clear();
   }

    std::vector<Location> PathTaken(Location& goal) { return ReconstructPath(start, goal, cameFrom); }

    bool IsStartSelected() const { return _startSelected; }
    const Location& GetStart() const { return start; }
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
    
    bool IsGoalSelected() const { return _goalSelected; }
    const Location& GetGoal() const { return goal; }
    void SetGoal(Location goal) {
        if (!_goalSelected) {
            _goalSelected = true;
            this->goal = goal;
        }
    }
    void ClearGoal() { _goalSelected = false; }
    
    const std::unordered_map<Location, Location>& GetCameFrom() const { return cameFrom; }
    const std::unordered_map<Location, unsigned int>& GetCostSoFar() const { return costSoFar; }
    const Cost_t GetCostAtLocation(Location loc) { return costSoFar[loc]; }
    const PriorityQueue<Location, unsigned int>& GetFrontier() const { return frontier; }

    const SearchRecord<Location>& GetCurrentRecord() { return record[currentRecord]; }
    void MoveAheadRecord() {currentRecord += (currentRecord < record.size()) ? 1 : 0; }
    void MoveBackwardRecord() { currentRecord -= (currentRecord > 0) ? 1 : 0; }

private:
    PathfindingStrategy<Graph>* strategy;

    Location start;
    bool _startSelected;
    Location goal;
    bool _goalSelected;
    
    std::unordered_map<Location, Location> cameFrom;
    std::unordered_map<Location, Cost_t> costSoFar;
    PriorityQueue<Location, Cost_t> frontier;

    std::vector<SearchRecord<Location>> record;
    unsigned int currentRecord;
};

template<typename Graph>
bool Pathfinder<Graph>::MakeStep(Graph& graph) {
    SearchRecord<Location> cRecord;
    cRecord.currentNode = frontier.top();
    cRecord.completed = strategy->MakeStep(graph, goal, cameFrom, costSoFar, frontier);
    
    for (auto it : cameFrom) {
        cRecord.cameFrom.push_back(it);
    }
    for (auto it : costSoFar) {
        cRecord.costSoFar.push_back(it);
    }
    PriorityQueue<Location, unsigned int> tmp = frontier;
    while (!tmp.isEmpty()) { //Render discovered and open nodes
        cRecord.frontier.push_back(tmp.get());
    }

    record.push_back(cRecord);
    
    return cRecord.completed;
}
