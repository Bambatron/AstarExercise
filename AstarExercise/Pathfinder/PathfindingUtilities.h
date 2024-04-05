#pragma once

#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>
#include <stack>
#include <queue>
#include <limits>

template<typename T, typename priority_t>
struct PriorityQueue {
    typedef std::pair<priority_t, T> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>,
        std::greater<PQElement>> elements;

    inline bool isEmpty() const {
        return elements.empty();
    }

    inline void put(T item, priority_t priority) {
        elements.emplace(priority, item);
    }

    T get() {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }

    T top() {
        return elements.top().second;
    }
};



template<typename location_t>
std::vector<location_t> ReconstructPath(location_t start, location_t goal, std::unordered_map<location_t, location_t> cameFrom) {
    std::vector<location_t> path;
    location_t current = goal;
    if (cameFrom.find(goal) == cameFrom.end()) {
        return path; // no path can be found
    }
    while (current != start) {
        path.push_back(current);
        current = cameFrom[current];
    }
    path.push_back(start); // optional
    std::reverse(path.begin(), path.end());
    return path;
}



template<typename Graph>
struct SearchRecord {
    typedef typename Graph::location_t location_t;
    typedef typename Graph::cost_t cost_t;

    bool completed;
    location_t currentNode;

    std::vector<std::pair<location_t, cost_t>> discovered;    // Seen but not entered
    std::vector<std::pair<location_t, cost_t>> visited;   // Seen and entered

    std::vector<location_t> pathToThisPoint;    // The path to take to get to the current node
};



// Interface for the strategies
template<typename Graph>
class PathfindingStrategy {
public:
    virtual const std::vector<SearchRecord<Graph>> MakeSearch(Graph& graph, typename Graph::location_t& start, typename Graph::location_t& goal, typename Graph::cost_t maxBudget) const = 0;

    virtual ~PathfindingStrategy() {} //Virtual destructor for polymorphism
};