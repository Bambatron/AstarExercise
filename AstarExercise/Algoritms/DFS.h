#pragma once

#include "../Pathfinder/PathfindingUtilities.h"

template<typename Graph>
void DepthFirstSearch(Graph& graph, typename Graph::location& start) {
    typedef typename Graph::location location;

    std::stack<location> frontier;
    frontier.push(start);

    std::set<location> reached;
    reached.insert(start);

    while (!frontier.empty()) {
        // Pop the current location from the top of the stack
        location current = frontier.top();
        frontier.pop();

        std::cout << "looking at: " << current.PrintOut() << std::endl;	 // Visualize the current location

        // For each neighbor if it has not been reached before
        for (location next : graph.Neighbors(current)) {
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
void DFSRecursiveStep(Graph& graph, typename Graph::location& current, std::set<typename Graph::location>& reached) {
    typedef typename Graph::location location;
    
    current.Read(); // Visualize the current location
    reached.insert(current);

    // For each neighbor if it has not been reached before
    for (location next : graph.Neighbors(current)) {
        if (reached.find(next) == reached.end()) {
            // Recursively perform DFS on the neighbor
            DFSRecursiveStep(graph, next, reached);
        }
    }
}

template<typename Graph>
void DFSRecursive(Graph& graph, typename Graph::location& start) {
    std::set<typename Graph::location> reached;
    DFSRecursiveStep(graph, start, reached);
}

template<typename Graph>
void DFSToGoal(Graph& graph, typename Graph::location& start, typename Graph::location& goal) {
    typedef typename Graph::location location;
    
    std::stack<location> frontier;
    frontier.push(start);

    std::set<location> reached;
    reached.insert(start);

    while (!frontier.empty()) {
        // Pop the current location from the top of the stack
        location current = frontier.top();
        frontier.pop();

        std::cout << "looking at: " << current.PrintOut() << std::endl;  // Visualize the current location

        // Check if the current location is the goal
        if (current == goal) {
            std::cout << "Goal found: " << current.PrintOut() << std::endl;
            break; // Exit the loop if goal is found
        }

        // For each neighbor if it has not been reached before
        for (location next : graph.Neighbors(current)) {
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
bool DFSToGoalStep(Graph& graph, typename Graph::location& goal, std::set<typename Graph::location>& reached, std::stack<typename Graph::location>& frontier) {
    typedef typename Graph::location location;
    
    if (frontier.empty()) {
        std::cout << "Frontier is empty" << std::endl;
        return true;
    }

    location current = frontier.top();
    frontier.pop();
    std::cout << "Looking at: " << current.PrintOut() << "\t"; // Visualize the current location

    // Check if the current location is the goal
    if (current == goal) {
        std::cout << "Goal found: " << current.PrintOut() << std::endl;
        return true; // Exit the recursion if the goal is found
    }

    // For each neighbor if it has not been reached before
    for (location next : graph.Neighbors(current)) {
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
void DFSToGoalHelper(Graph& graph, typename Graph::location& goal, std::set<typename Graph::location>& reached, std::stack<typename Graph::location>& frontier) {
    if (!DFSToGoalStep(graph, goal, reached, frontier)) {
        DFSToGoalHelper(graph, goal, reached, frontier);
    }
}

template<typename Graph>
void DFSToGoalRecursive(Graph& graph, typename Graph::location& start, typename Graph::location& goal) {
    typedef typename Graph::location location;

    std::stack<location> frontier;
    frontier.push(start);

    std::set<location> reached;
    reached.insert(start);

    DFSToGoalHelper(graph, goal, reached, frontier);
}
