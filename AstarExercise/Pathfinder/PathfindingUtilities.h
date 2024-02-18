#pragma once

#include <iostream>
#include <unordered_map>
#include <set>
#include <stack>
#include <queue>

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

template<typename Location>
std::vector<Location> ReconstructPath(Location start, Location goal, std::unordered_map<Location, Location> cameFrom) {
    std::vector<Location> path;
    Location current = goal;
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


// Interface for 
template<typename Graph>
class PathfindingStrategy {
public:
 //   static virtual PathfindingStrategy* CreateInstace(Graph& graph, Location& loc) = 0;
    
    virtual bool MakeStep(Graph& graph, typename Graph::Location& goal,
        std::unordered_map<typename Graph::Location, typename Graph::Location>& cameFrom,
        std::unordered_map<typename Graph::Location, typename Graph::Cost_t>& costSoFar,
        PriorityQueue<typename Graph::Location, typename Graph::Cost_t>& frontier) = 0;

    virtual ~PathfindingStrategy() {} // Virtual destructor for polymorphism
};