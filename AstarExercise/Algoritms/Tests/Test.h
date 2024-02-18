#pragma once

#include <iostream>
#include <cassert>
#include <fstream>

#include "../json.hpp"

#include "../Algoritms/Astar.h"

// Define your test cases
template<typename Graph>
void TestValidPath(const nlohmann::json& caseObject) {
    // Define your graph (assuming it's represented somehow, e.g., adjacency list)
    nlohmann::json graphObject = caseObject["graph"];
    Graph grpah(graphObject);
    typedef typename Graph::Location location;
    typedef typename Graph::Cost_t cost;

    location start(caseObject["start"]["q"], caseObject["start"]["r"]);    
    location goal(caseObject["goal"]["q"], caseObject["goal"]["r"]);
    
    std::unordered_map<location, location> cameFrom;
    cameFrom[start] = start;
    std::unordered_map<location, cost> costSoFar;
    costSoFar[start] = 0;
    PriorityQueue<location, cost> frontier;
    frontier.put(start, 0);

    cost expectedCost = caseObject["expectedCost"];

    while (!AstarSearchStep(graph, goal, cameFrom, costSoFar, frontier)) {}

    std::vector<location> pathTaken = ReconstructPath(start, goal, cameFrom);

    if (costSoFar[goal] != expectedCost) {
        std::cerr << "Test case failed: Calculated cost (" << costSoFar[goal]
            << ") is not equal to the expected cost (" << expectedCost << ")" << std::endl;
    }
    assert(costSoFar[goal] == expectedCost);   //check if path is correct 
}

template<typename Graph>
void TestNoPath(const nlohmann::json& caseObject) {
    // Define your graph (assuming it's represented somehow, e.g., adjacency list)
    Graph graph(/* define your graph */);
   

    // Call your A* search function
    std::vector<Node> path = astar_search(graph, /* start node */, /* goal node */);

    // Verify that no path exists
    assert(path.empty());
}

template<typename Graph>
void TestInvalidInput(const nlohmann::json& caseObject) {
    // Define your graph (assuming it's represented somehow, e.g., adjacency list)
    Graph graph(/* define your graph */);

    // Call your A* search function with invalid input
    std::vector<Node> path = astar_search(graph, /* invalid start node */, /* invalid goal node */);

    // Verify that the output is handled correctly (e.g., error handling)
    assert(path.empty()); // or whatever error handling mechanism you have
}

template<typename Graph>
void RunTest(const std::string& jsonFilePath = "AstarHexTest.json") {
    std::ifstream file(jsonFilePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << jsonFilePath << std::endl;
    }
    // Parse the JSON data
    nlohmann::json jsonData;
    try {
        file >> jsonData;

        nlohmann::json validCaseObject = jsonData["validCase"];
        TestValidPath<Graph>(validCaseObject);

        nlohmann::json noPathCaseObject = jsonData["noPathCase"];
        TestNoPath<Graph>(noPathCaseObject);

        nlohmann::json invalidCaseObject = jsonData["invalidCase"];
        TestInvalidInput<Graph>(invalidCaseObject);
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
}