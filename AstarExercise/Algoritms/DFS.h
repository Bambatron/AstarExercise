#pragma once

#include "../Pathfinder/PathfindingUtilities.h"

template<typename Graph>
void DepthFirstSearch(Graph& graph, typename Graph::location_t& start) {
    typedef typename Graph::location_t location_t;

    std::stack<location_t> frontier;
    frontier.push(start);

    std::set<location_t> reached;
    reached.insert(start);

    while (!frontier.empty()) {
        // Pop the current location_t from the top of the stack
        location_t current = frontier.top();
        frontier.pop();

        std::cout << "looking at: " << current.PrintOut() << std::endl;	 // Visualize the current location_t

        // For each neighbor if it has not been reached before
        for (location_t next : graph.Neighbors(current)) {
            if (reached.find(next) == reached.end()) {
                std::cout << "Found not dicovered neighbor: " << next.PrintOut() << "\t";
                // Add the neighbor to the stack and mark it as reached
                frontier.push(next);
                reached.insert(next);
            }
        }
        std::cout << std::endl;
    }
}

template<typename Graph>
void DFSRecursiveStep(Graph& graph, typename Graph::location_t& current, std::set<typename Graph::location_t>& reached) {
    typedef typename Graph::location_t location_t;
    
    current.Read(); // Visualize the current location
    reached.insert(current);

    // For each neighbor if it has not been reached before
    for (location_t next : graph.Neighbors(current)) {
        if (reached.find(next) == reached.end()) {
            // Recursively perform DFS on the neighbor
            DFSRecursiveStep(graph, next, reached);
        }
    }
}

template<typename Graph>
void DFSRecursive(Graph& graph, typename Graph::locatlocation_tion& start) {
    std::set<typename Graph::location> reached;
    DFSRecursiveStep(graph, start, reached);
}

template<typename Graph>
void DFSToGoal(Graph& graph, typename Graph::location_t& start, typename Graph::location_t& goal) {
    typedef typename Graph::location_t location_t;
    
    std::stack<location_t> frontier;
    frontier.push(start);

    std::set<location_t> reached;
    reached.insert(start);

    while (!frontier.empty()) {
        // Pop the current location from the top of the stack
        location_t current = frontier.top();
        frontier.pop();

        std::cout << "looking at: " << current.PrintOut() << std::endl;  // Visualize the current location

        // Check if the current location is the goal
        if (current == goal) {
            std::cout << "Goal found: " << current.PrintOut() << std::endl;
            break; // Exit the loop if goal is found
        }

        // For each neighbor if it has not been reached before
        for (location_t next : graph.Neighbors(current)) {
            if (reached.find(next) == reached.end()) {
                std::cout << "Found not discovered neighbor: " << next.PrintOut() << "\t";
                // Add the neighbor to the stack and mark it as reached
                frontier.push(next);
                reached.insert(next);
            }
        }
        std::cout << std::endl;
    }

    std::cout << "Goal not found." << std::endl;
}

template<typename Graph>
bool DFSToGoalStep(Graph& graph, typename Graph::location_t& goal, std::set<typename Graph::location_t>& reached, std::stack<typename Graph::location_t>& frontier) {
    typedef typename Graph::location_t location_t;
    
    if (frontier.empty()) {
        std::cout << "Frontier is empty" << std::endl;
        return true;
    }

    location_t current = frontier.top();
    frontier.pop();
    std::cout << "Looking at: " << current.PrintOut() << "\t"; // Visualize the current location

    // Check if the current location is the goal
    if (current == goal) {
        std::cout << "Goal found: " << current.PrintOut() << std::endl;
        return true; // Exit the recursion if the goal is found
    }

    // For each neighbor if it has not been reached before
    for (location_t next : graph.Neighbors(current)) {
        if (reached.find(next) == reached.end()) {
            std::cout << "Found not dicovered neighbor: " << next.PrintOut() << "\t";
            // Add the neighbor to the stack and mark it as reached
            frontier.push(next);
            reached.insert(next);
        }
    }
    
    return false;
}

template<typename Graph>
void DFSToGoalHelper(Graph& graph, typename Graph::location_t& goal, std::set<typename Graph::location_t>& reached, std::stack<typename Graph::location_t>& frontier) {
    if (!DFSToGoalStep(graph, goal, reached, frontier)) {
        DFSToGoalHelper(graph, goal, reached, frontier);
    }
}

template<typename Graph>
void DFSToGoalRecursive(Graph& graph, typename Graph::location_t& start, typename Graph::location_t& goal) {
    typedef typename Graph::location_t location_t;

    std::stack<location_t> frontier;
    frontier.push(start);

    std::set<location_t> reached;
    reached.insert(start);

    DFSToGoalHelper(graph, goal, reached, frontier);
}
