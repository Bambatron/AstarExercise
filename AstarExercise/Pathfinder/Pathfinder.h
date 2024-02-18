#pragma once

#include "../Algoritms/BFS.h"
#include "../Algoritms/DFS.h"
#include "../Algoritms/Dijkstra.h"
#include "../Algoritms/Astar.h"

template<typename Location>
struct SearchRecord {
    bool completed;
    Location currentNode;
    std::vector<std::pair<Location, Location>> visited;    //pair<Next, Current>
    std::vector<std::pair<Location, unsigned int>> costSoFar;   //Cost to go to each location
    std::vector<Location> discovered;    //Discovered noedds
};

template <typename Graph>
class Pathfinder {
public:
    using Location = typedef typename Graph::Location;
    using Cost_t = typedef typename Graph::Cost_t;

	Pathfinder(PathfindingStrategy<Graph>* _strat) : strategy(_strat) {
        _searchCompleted = false;
        _startSelected = false;
        _goalSelected = false;
        currentRecord = 0;
    }

    void MakeSearch(Graph& graph);
    bool MakeStep(Graph& graph);

    void SwitchFuntion(PathfindingStrategy<Graph>* _strat) { this->strategy = _strat; }

   void Reset() {
       _searchCompleted = false;
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

    bool IsSearchCompleted() const { return _searchCompleted; }

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
    void MoveAheadRecord() {currentRecord += (currentRecord < record.size()-1) ? 1 : 0; }
    void MoveBackwardRecord() { currentRecord -= (currentRecord > 0) ? 1 : 0; }

private:
    PathfindingStrategy<Graph>* strategy;

    bool _searchCompleted;

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

template <typename Graph>
void Pathfinder<Graph>::MakeSearch(Graph& graph) {
    if (_startSelected && _goalSelected && !_searchCompleted) {
        while (!_searchCompleted) {
            _searchCompleted = MakeStep(graph);
        }
    }
}

template<typename Graph>
bool Pathfinder<Graph>::MakeStep(Graph& graph) {
    SearchRecord<Location> cRecord;
    cRecord.currentNode = frontier.top();
    cRecord.completed = strategy->MakeStep(graph, goal, cameFrom, costSoFar, frontier);
    
    for (auto it : cameFrom) {
        cRecord.visited.push_back(it);
    }
    for (auto it : costSoFar) {
        cRecord.costSoFar.push_back(it);
    }
    PriorityQueue<Location, unsigned int> tmp = frontier;
    while (!tmp.isEmpty()) { //Render discovered and open nodes
        cRecord.discovered.push_back(tmp.get());
    }

    record.push_back(cRecord);
    
    return cRecord.completed;
}
