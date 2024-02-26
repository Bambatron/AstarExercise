#include "pch.h"
#include "CppUnitTest.h"

#include <iostream>
#include <cassert>
#include <fstream>

#include "../Common/json.hpp"

#include "../AstarExercise/Algoritms/Astar.h"
#include "../AstarExercise/Grids/HexGrid.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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

namespace AstarUnitTest {
	TEST_CLASS(AstarUnitTest)
	{
	public:        
        TEST_METHOD(ValidPath) {
            //Make graph
            HexGrid graph(nlohmann::json{
                {"origin", {
                    {"q", 0},
                    {"r", 0}
                }},
                {"radius", 2},
                {"weightedNodes", {
                    {{"q", 0}, {"r", -1}, {"w", 4}},
                    {{"q", -1}, {"r", 0}, {"w", 4}},
                    {{"q", -2}, {"r", 0}, {"w", 4}},
                    {{"q", 0}, {"r", -1}, {"w", 4}},
                    {{"q", 1}, {"r", 0}, {"w", 8}},
                    {{"q", 0}, {"r", 1}, {"w", 5}},
                    {{"q", 1}, {"r", 1}, {"w", 7}}
                }}});

            typedef typename HexGrid::location location;
            typedef typename HexGrid::cost_t cost;

            //Make necessiteies for the search
            location start(0, 0);
            location goal(-1, -1);
            std::vector<location> pathTaken;

            cost expectedCost = 8;

            std::vector<SearchRecord<HexGrid>> searchResult;
            searchResult = AstarSearchRecorded(graph, start, goal, pathTaken);

            // Process path
            SearchRecord<HexGrid> lastRecord = searchResult[searchResult.size() - 1];
            cost finalCost;
            for (const auto& visited : lastRecord.alreadyVisited) {
                if (visited.first == goal) {
                    finalCost = visited.second;
                }
            }

            // Test
            Assert::AreEqual(finalCost, expectedCost);
        }

        TEST_METHOD(OutOfBoundStart) {
            HexGrid graph(2, Hex(0, 0), true);
            typedef typename HexGrid::location location;
            typedef typename HexGrid::cost_t cost;
            
            location start(3, 0);
            location goal(0, 0);
            std::vector<location> pathTaken;

            Assert::ExpectException<std::runtime_error>([&]() { AstarSearchRecorded(graph, start, goal, pathTaken); });
        }

        TEST_METHOD(OutOfBoundGoal) {
            HexGrid graph(2, Hex(0, 0), true);
            typedef typename HexGrid::location location;
            typedef typename HexGrid::cost_t cost;

            location start(0, 0);
            location goal(-3, 0);
            std::vector<location> pathTaken;

            Assert::ExpectException<std::runtime_error>([&]() { AstarSearchRecorded(graph, start, goal, pathTaken); });
        }
	};
}
