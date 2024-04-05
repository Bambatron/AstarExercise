#pragma once

#include "../Algoritms/BFS.h"
#include "../Algoritms/DFS.h"
#include "../Algoritms/Dijkstra.h"
#include "../Algoritms/Astar.h"

template <typename Graph>
class Pathfinder {
public:
    using location = typedef typename Graph::location_t;
    using cost_t = typedef typename Graph::cost_t;

	Pathfinder(PathfindingStrategy<Graph>* _strat) : strategy(_strat) {
        _searchCompleted = false;
        _startSelected = false;
        _goalSelected = false;
        maxBudget = 100;
        currentRecord = 0;
    }

    void MakeSearch(Graph& graph);

    // Getters
    bool IsSearchCompleted() const { return _searchCompleted; }

    bool IsStartSelected() const { return _startSelected; }
    const location& GetStart() const { return start; }

    bool IsGoalSelected() const { return _goalSelected; }
    const location& GetGoal() const { return goal; }

    cost_t getMaxBudget() const { return maxBudget; }

    const SearchRecord<Graph>& GetCurrentRecord() { return record[currentRecord]; }

    // Setters
    void SwitchFuntion(PathfindingStrategy<Graph>* _strat) { this->strategy = _strat; }
    
    void Reset() {
        _searchCompleted = false;
        _startSelected = false;
        _goalSelected = false;
        currentRecord = 0;
        record.clear();
    }

    void SetStart(location start) {
        if (!_startSelected) {
            _startSelected = true;
            this->start = start;
        }
    }
    void ClearStart() { _startSelected = false; }

    void SetGoal(location goal) {
        if (!_goalSelected) {
            _goalSelected = true;
            this->goal = goal;
        }
    }
    void ClearGoal() { _goalSelected = false; }

    void setMaxBudget(cost_t maxBudget) { this->maxBudget = maxBudget; }

    void MoveAheadRecord() {currentRecord += (currentRecord < record.size()-1) ? 1 : 0; }
    void MoveBackwardRecord() { currentRecord -= (currentRecord > 0) ? 1 : 0; }

private:
    PathfindingStrategy<Graph>* strategy;

    bool _searchCompleted;

    location start;
    bool _startSelected;
    location goal;
    bool _goalSelected;
    
    cost_t maxBudget;

    std::vector<SearchRecord<Graph>> record;
    unsigned int currentRecord;
};

template <typename Graph>
void Pathfinder<Graph>::MakeSearch(Graph& graph) {
    this->record = strategy->MakeSearch(graph, start, goal, maxBudget);
    currentRecord = 0 ;
    _searchCompleted = true;
}
