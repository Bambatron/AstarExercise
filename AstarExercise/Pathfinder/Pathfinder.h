#pragma once

#include "../Algoritms/BFS.h"
#include "../Algoritms/DFS.h"
#include "../Algoritms/Dijkstra.h"
#include "../Algoritms/Astar.h"

template <typename Graph>
class Pathfinder {
public:
    using location = typedef typename Graph::location;
    using cost_t = typedef typename Graph::cost_t;

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

    std::vector<location> PathTaken(location& goal) { return ReconstructPath(start, goal, cameFrom); }

    bool IsSearchCompleted() const { return _searchCompleted; }

    bool IsStartSelected() const { return _startSelected; }
    const location& GetStart() const { return start; }
    void SetStart(location start) {
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
    const location& GetGoal() const { return goal; }
    void SetGoal(location goal) {
        if (!_goalSelected) {
            _goalSelected = true;
            this->goal = goal;
        }
    }
    void ClearGoal() { _goalSelected = false; }
    
    const std::unordered_map<location, location>& GetCameFrom() const { return cameFrom; }
    const std::unordered_map<location, cost_t >& GetCostSoFar() const { return costSoFar; }
    const cost_t  GetCostAtLocation(location loc) { return costSoFar[loc]; }
    const PriorityQueue<location, cost_t>& GetFrontier() const { return frontier; }

    const SearchRecord<Graph>& GetCurrentRecord() { return record[currentRecord]; }
    void MoveAheadRecord() {currentRecord += (currentRecord < record.size()-1) ? 1 : 0; }
    void MoveBackwardRecord() { currentRecord -= (currentRecord > 0) ? 1 : 0; }

private:
    PathfindingStrategy<Graph>* strategy;

    bool _searchCompleted;

    location start;
    bool _startSelected;
    location goal;
    bool _goalSelected;
    
    std::unordered_map<location, location> cameFrom;
    std::unordered_map<location, cost_t > costSoFar;
    PriorityQueue<location, cost_t > frontier;

    std::vector<SearchRecord<Graph>> record;
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
    SearchRecord<Graph> cRecord;
    cRecord.currentNode = frontier.top();
    cRecord.completed = strategy->MakeStep(graph, goal, cameFrom, costSoFar, frontier);
    
    for (auto it : costSoFar) {
        cRecord.visited.push_back(std::make_pair(it.first, it.second));
    }
    PriorityQueue<location, cost_t > tmp = frontier;
    while (!tmp.isEmpty()) { //Render discovered and open nodes
        location it = tmp.get();
        cRecord.discovered.push_back(std::make_pair(it, graph.Weight(it)));
    }

    record.push_back(cRecord);
    
    return cRecord.completed;
}
