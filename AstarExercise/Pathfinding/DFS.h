#pragma once

#include "PathfindingUtilities.h"

template<typename Graph, typename Location>
void DepthFirstSearch(Graph& graph, Location& start) {
    std::stack<Location> frontier;
    frontier.push(start);

    std::set<Location> reached;
    reached.insert(start);

    while (!frontier.empty()) {
        // Pop the current location from the top of the stack
        Location current = frontier.top();
        frontier.pop();

        std::cout << "looking at: " << current.PrintOut() << std::endl;	 // Visualize the current location

        // For each neighbor if it has not been reached before
        for (Location next : graph.Neighbors(current)) {
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

template<typename Graph, typename Location>
void DFSRecursiveStep(Graph& graph, Location& current, std::set<Location>& reached) {
    current.Read(); // Visualize the current location
    reached.insert(current);

    // For each neighbor if it has not been reached before
    for (Location next : graph.Neighbors(current)) {
        if (reached.find(next) == reached.end()) {
            // Recursively perform DFS on the neighbor
            DFSRecursiveStep(graph, next, reached);
        }
    }
}

template<typename Graph, typename Location>
void DFSRecursive(Graph& graph, Location& start) {
    std::set<Location> reached;
    DFSRecursiveStep(graph, start, reached);
}

template<typename Graph, typename Location>
void DFSToGoal(Graph& graph, Location& start, Location& goal) {
    std::stack<Location> frontier;
    frontier.push(start);

    std::set<Location> reached;
    reached.insert(start);

    while (!frontier.empty()) {
        // Pop the current location from the top of the stack
        Location current = frontier.top();
        frontier.pop();

        std::cout << "looking at: " << current.PrintOut() << std::endl;  // Visualize the current location

        // Check if the current location is the goal
        if (current == goal) {
            std::cout << "Goal found: " << current.PrintOut() << std::endl;
            break; // Exit the loop if goal is found
        }

        // For each neighbor if it has not been reached before
        for (Location next : graph.Neighbors(current)) {
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

template<typename Graph, typename Location>
bool DFSToGoalStep(Graph& graph, Location& goal, std::set<Location>& reached, std::stack<Location>& frontier) {
    if (frontier.empty()) {
        std::cout << "Frontier is empty" << std::endl;
        return true;
    }

    Location current = frontier.top();
    frontier.pop();
    std::cout << "Looking at: " << current.PrintOut() << "\t"; // Visualize the current location

    // Check if the current location is the goal
    if (current == goal) {
        std::cout << "Goal found: " << current.PrintOut() << std::endl;
        return true; // Exit the recursion if the goal is found
    }

    // For each neighbor if it has not been reached before
    for (Location next : graph.Neighbors(current)) {
        if (reached.find(next) == reached.end()) {
            std::cout << "Found not dicovered neighbor: " << next.PrintOut() << "\t";
            // Add the neighbor to the stack and mark it as reached
            frontier.push(next);
            reached.insert(next);
        }
    }
    
    return false;
}

template<typename Graph, typename Location>
void DFSToGoalHelper(Graph& graph, Location& goal, std::set<Location>& reached, std::stack<Location>& frontier) {
    if (!DFSToGoalStep(graph, goal, reached, frontier)) {
        DFSToGoalHelper(graph, goal, reached, frontier);
    }
}

template<typename Graph, typename Location>
void DFSToGoalRecursive(Graph& graph, Location& start, Location& goal) {
    std::stack<Location> frontier;
    frontier.push(start);

    std::set<Location> reached;
    reached.insert(start);

    DFSToGoalHelper(graph, goal, reached, frontier);
}
