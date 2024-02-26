#pragma once

#include <iostream>
#include <cassert>
#include <fstream>

#include "../../json.hpp"

#include "../Astar.h"

// Define your test cases
template<typename Graph>
bool TestAstarValidPath(const nlohmann::json& caseObject) {
    //Read Graph, Start and Goal
    nlohmann::json graphObject = caseObject["graph"];
    std::cout << graphObject << std::endl;
    Graph graph(graphObject);
    typedef typename Graph::location location;
    typedef typename Graph::cost_t cost;

    location start(caseObject["start"]["q"], caseObject["start"]["r"]);
    location goal(caseObject["goal"]["q"], caseObject["goal"]["r"]);

    std::vector<location> pathTaken;

    //Read expected results of the search
    cost expectedCost = caseObject["expectedCost"];
    std::vector<location> expectedPath;
    bool _expectedPathPresent = false;
    if (caseObject.find("expectedPath") != caseObject.end()) {
        _expectedPathPresent = true;
        for (const auto& obj : caseObject["expectedPath"]) {
            location loc(obj);
            expectedPath.push_back(loc);
        }
    }

    std::vector<SearchRecord<Graph>> searchResult;
    
    try {   //Executes search
        searchResult = AstarSearchRecorded(graph, start, goal, pathTaken);
        
        // Process path
        SearchRecord<Graph> lastRecord = searchResult[searchResult.size() - 1];

        if (_expectedPathPresent) {
            if (pathTaken.size() != expectedPath.size()) {
                std::cerr << "Test case failed: path found size (" << pathTaken.size() << ") is not equal to the expected path size (" << expectedPath.size() << ")" << std::endl;
                return false;
            }

            for (int i = 0; i < pathTaken.size(); i++) {
                if (pathTaken[i] != expectedPath[i]) {
                    std::cerr << "Test case failed: Node taken (" << pathTaken[i].PrintOut() << ") is not equal to the expected node (" << expectedPath[i].PrintOut() << ")" << std::endl;
                    return false;
                }
            }
        }
        return true;

    }
    catch (const std::runtime_error& e) {   // Handle runtime_error exception
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e) {   // Handle other exceptions
        std::cerr << "Unknown exception caught: " << e.what() << std::endl;
        return false;
    }
}

template<typename Graph>
bool TestAstarInvalidInput(const nlohmann::json& caseObject) {  //Either start or goal are no good
    //Read Graph, Start and Goal
    nlohmann::json graphObject = caseObject["graph"];
    Graph graph(graphObject);
    typedef typename Graph::location location;
    typedef typename Graph::cost_t cost;

    location start(caseObject["start"]["q"], caseObject["start"]["r"]);
    location goal(caseObject["goal"]["q"], caseObject["goal"]["r"]);

    std::vector<location> pathTaken;

    //Read expected results of the search
    cost expectedCost = caseObject["expectedCost"];
    std::vector<location> expectedPath;
    bool _expectedPathPresent = false;
    if (caseObject.find("expectedPath") != caseObject.end()) {
        _expectedPathPresent = true;
        for (const auto& obj : caseObject["expectedPath"]) {
            location loc(obj);
            expectedPath.push_back(loc);
        }
    }

    std::vector<SearchRecord<Graph>> searchResult;

    try {   //Executes search
        searchResult = AstarSearchRecorded(graph, start, goal, pathTaken);

        std::cerr << "Test case failed: Algorithm didn't stop  with an invalid case" << std::endl;
        return false;
    }
    catch (const std::runtime_error& e) {   // Handle runtime_error exception
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return true;
    }
    catch (const std::exception& e) {   // Handle other exceptions
        std::cerr << "Unknown exception caught: " << e.what() << std::endl;
        return false;
    }
}

template<typename Graph>
bool RunAstarTest(const std::string& jsonFilePath = "AstarHexTest.json") {
    std::ifstream file(jsonFilePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << jsonFilePath << std::endl;
    }
    // Parse the JSON data
    nlohmann::json jsonData;
    bool ret = false;
    try {
        file >> jsonData;

        if (jsonData.find("validCase") != jsonData.end()) {
            std::cout << "Testing for valid path case" << std::endl;
            nlohmann::json validCaseObject = jsonData["validCase"];
            
            ret = TestAstarValidPath<Graph>(validCaseObject);
            if (ret) {
                std::cerr << "Test for valid path case: SUCCESSFUL" << std::endl;
            }
        }

        if (jsonData.find("invalidCase") != jsonData.end()) {
            std::cout << "Testing for invalid path case" << std::endl;
            nlohmann::json invalidCaseObject = jsonData["invalidCase"];
            ret = TestAstarInvalidInput<Graph>(invalidCaseObject);
            if (ret) {
                std::cerr << "Test for invalid path case: SUCCESSFUL" << std::endl;
            }
        }

        return ret;
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return false;
    }

   
}