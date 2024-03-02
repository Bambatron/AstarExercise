#include "pch.h"
#include "CppUnitTest.h"

#include <iostream>
#include <cassert>
#include <fstream>

#include "../Common/json.hpp"

#include "../AstarExercise/Algoritms/Astar.h"
#include "../AstarExercise/Grids/HexGrid.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AstarUnitTest {
	TEST_CLASS(AstarUnitTest)
	{
	public:        
        TEST_METHOD(ValidPath) {
            //Make graph
            HexGrid graph;
            graph.LoadFromString(nlohmann::json{
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
            for (const auto& vs : lastRecord.visited) {
                if (vs.first == goal) {
                    finalCost = vs.second;
                }
            }

            // Test
            Assert::AreEqual(finalCost, expectedCost);
            Logger::WriteMessage("Astar Test: valid path is passed");
            std::cout << "Astar Test: valid path is passed" << std::endl;
        }

        TEST_METHOD(OutOfBoundStart) {
            HexGrid graph(2, Hex(0, 0), true);
            typedef typename HexGrid::location location;
            typedef typename HexGrid::cost_t cost;
            
            location start(3, 0);
            location goal(0, 0);
            std::vector<location> pathTaken;

            Assert::ExpectException<std::runtime_error>([&]() { AstarSearchRecorded(graph, start, goal, pathTaken); });
            Logger::WriteMessage("Astar Test: invalid path is passed");
            std::cout << "Astar Test: invalid path is passed" << std::endl;
        }

        TEST_METHOD(OutOfBoundGoal) {
            HexGrid graph(2, Hex(0, 0), true);
            typedef typename HexGrid::location location;
            typedef typename HexGrid::cost_t cost;

            location start(0, 0);
            location goal(-3, 0);
            std::vector<location> pathTaken;

            Assert::ExpectException<std::runtime_error>([&]() { AstarSearchRecorded(graph, start, goal, pathTaken); });
            Logger::WriteMessage("Astar Test: invalid path is passed");
            std::cout << "Astar Test: invalid path is passed" << std::endl;
        }
	};
}
