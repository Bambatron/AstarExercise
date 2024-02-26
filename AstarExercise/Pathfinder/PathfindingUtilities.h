#pragma once

#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>
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



template<typename Graph>
struct SearchRecord {
    using location = typedef typename Graph::location;
    using cost_t = typedef typename Graph::cost_t;

    bool completed;
    location currentNode;

    std::vector<std::pair<location, cost_t>> toBeVisited;
    std::vector<std::pair<location, cost_t>> alreadyVisited;
};



// Interface for 
template<typename Graph>
class PathfindingStrategy {
public:
    virtual bool MakeStep(Graph& graph, typename Graph::location& goal,
        std::unordered_map<typename Graph::location, typename Graph::location>& cameFrom,
        std::unordered_map<typename Graph::location, typename Graph::cost_t>& costSoFar,
        PriorityQueue<typename Graph::location, typename Graph::cost_t>& frontier) = 0;

    virtual std::vector<SearchRecord<Graph>> MakeSearch(Graph& graph, typename Graph::location& start, typename Graph::location& goal, std::vector<typename Graph::location>& pathTaken) = 0;

        virtual ~PathfindingStrategy() {} // Virtual destructor for polymorphism
};