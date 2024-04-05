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
	TEST_CLASS(AstarUnitTest) {
	public:
        template<typename T>
        bool vectorsEqual(const std::vector<T>& v1, const std::vector<T>& v2) {
            if (v1.size() != v2.size()) {
                return false;
            }

            for (size_t i = 0; i < v1.size(); ++i) {
                if (v1[i] != v2[i]) {
                    return false;
                }
            }

            return true;
        }

        TEST_METHOD(ValidPath) {
            Logger::WriteMessage("Astar Test: starting valid path test\n");
            // Make graph
            HexGrid graph;
            graph.LoadFromString(nlohmann::json{
                {"origin", {
                    {"q", 0},
                    {"r", 0}
                }},
                {"radius", 2},
                {"weightedNodes", {
                    {{"q", +0}, {"r", -1}, {"w", 4}},
                    {{"q", -1}, {"r", +0}, {"w", 4}},
                    {{"q", -2}, {"r", +0}, {"w", 4}},
                    {{"q", +0}, {"r", -1}, {"w", 4}},
                    {{"q", +1}, {"r", +0}, {"w", 8}},
                    {{"q", +0}, {"r", +1}, {"w", 5}},
                    {{"q", +1}, {"r", +1}, {"w", 7}}
                }}
            });

            typedef typename HexGrid::location_t location_t;
            typedef typename HexGrid::cost_t cost_t;

            //Make necessiteies for the search
            location_t start(0, 0);
            location_t goal(-1, -1);

            cost_t expectedCost = 8;
            std::vector<location_t> expectedPath;
            expectedPath.push_back(start);
            expectedPath.push_back(location_t(+1, -1));
            expectedPath.push_back(location_t(+1, -2));
            expectedPath.push_back(goal);

            std::vector<SearchRecord<HexGrid>> searchResult;
            searchResult = AstarSearchRecorded(graph, start, goal);

            // Process path
            SearchRecord<HexGrid> lastRecord = searchResult[searchResult.size() - 1];
            cost_t finalCost;
            for (const auto& vs : lastRecord.visited) {
                if (vs.first == goal) {
                    finalCost = vs.second;
                }
            }
            std::vector<location_t> pathTaken = lastRecord.pathToThisPoint;

            // Test
            assert(vectorsEqual(pathTaken, expectedPath));  // Checks path
            if (finalCost == expectedCost) {
                Logger::WriteMessage("Astar Test: valid path is passed");
            }
            Assert::AreEqual(finalCost, expectedCost);  // Checks cost
        }

        TEST_METHOD(ImpossiblePath) {
            Logger::WriteMessage("Astar Test: starting impossible path test\n");
            // Make graph
            HexGrid graph;
            graph.LoadFromString(nlohmann::json{
                {"origin", {
                    {"q", 0},
                    {"r", 0}
                }},
                {"radius", 2},
                {"weightedNodes", {
                    {{"q", +4}, {"r", +0}, {"w", 2}},
                    {{"q", +0}, {"r", -1}, {"w", 4}},
                    {{"q", -1}, {"r", +0}, {"w", 4}},
                    {{"q", -2}, {"r", +0}, {"w", 4}},
                    {{"q", +0}, {"r", -1}, {"w", 4}},
                    {{"q", +1}, {"r", +0}, {"w", 8}},
                    {{"q", +0}, {"r", +1}, {"w", 5}},
                    {{"q", +1}, {"r", +1}, {"w", 7}}
                }}
                });

            typedef typename HexGrid::location_t location_t;
            typedef typename HexGrid::cost_t cost_t;

            //Make necessiteies for the search
            location_t start(0, 0);
            location_t goal(4, 0);

            std::vector<SearchRecord<HexGrid>> searchResult;
            searchResult = AstarSearchRecorded(graph, start, goal);

            // Test
            for (const auto& r : searchResult) {
                Assert::AreEqual(r.completed, false);  // Checks that the search was never completed
            }
            Logger::WriteMessage("Astar Test: Algortim has not found an impossible path and stopped");
        }

        TEST_METHOD(OutOfBoundStart) {
            Logger::WriteMessage("Astar Test: starting out of bound start test\n");

            HexGrid graph(2, Hex(0, 0), true);
            typedef typename HexGrid::location_t location;
            typedef typename HexGrid::cost_t cost;
            
            location start(3, 0);
            location goal(0, 0);
            std::vector<location> pathTaken;

            try {
                AstarSearchRecorded(graph, start, goal);
                // If the method didn't throw any exception, the test should fail
                Logger::WriteMessage("Astar Test: invalid path is not passed. Exception was not thrown");
                Assert::Fail(L"Expected exception was not thrown.");
            }
            catch (std::runtime_error e) {
                Logger::WriteMessage("Astar Test: invalid path is passed");
                //Assert::AreEqual("ExpectedExceptionMessage", ex.what()); // Check the exception message if needed
            }
            catch (...) {
                Logger::WriteMessage("Astar Test: invalid path is not passed. Unknown exception was thrown");
                Assert::Fail(L"Unexpected exception was caught.");
            }
        }

        TEST_METHOD(OutOfBoundGoal) {
            Logger::WriteMessage("Astar Test: starting out of bound goal test\n");
            HexGrid graph(2, Hex(0, 0), true);
            typedef typename HexGrid::location_t location;
            typedef typename HexGrid::cost_t cost;

            location start(0, 0);
            location goal(-3, 0);
            std::vector<location> pathTaken;

            try {
                AstarSearchRecorded(graph, start, goal);
                // If the method didn't throw any exception, the test should fail
                Logger::WriteMessage("Astar Test: invalid path is not passed. Exception was not thrown");
                Assert::Fail(L"Expected exception was not thrown.");
            }
            catch (std::runtime_error e) {
                Logger::WriteMessage("Astar Test: invalid path is passed");
                //Assert::AreEqual("ExpectedExceptionMessage", ex.what()); // Check the exception message if needed
            }
            catch (...) {
                Logger::WriteMessage("Astar Test: invalid path is not passed. Unknown exception was thrown");
                Assert::Fail(L"Unexpected exception was caught.");
            }
        }

        TEST_METHOD(ExpensivePath) {
            Logger::WriteMessage("Astar Test: starting expensive path test\n");
            // Make graph
            HexGrid graph;
            graph.LoadFromString(nlohmann::json{
                {"origin", {
                    {"q", 0},
                    {"r", 0}
                }},
                {"radius", 2},
                {"weightedNodes", {
                    {{"q", +1}, {"r", -2}, {"w", 50}},
                    {{"q", +0}, {"r", -1}, {"w", 50}},
                    {{"q", -1}, {"r", -1}, {"w", 50}},
                    }}
                });

            typedef typename HexGrid::location_t location_t;
            typedef typename HexGrid::cost_t cost_t;

            //Make necessiteies for the search
            location_t start(0, 0);
            location_t goal(0, -2);

            std::vector<SearchRecord<HexGrid>> searchResult;
            searchResult = AstarSearchRecorded(graph, start, goal, 50);

            // Test
            for (const auto& r : searchResult) {
                Assert::AreEqual(r.completed, false);  // Checks that the search was never completed
            }
            Logger::WriteMessage("Astar Test: Algortim has not found a path cheap eanough to the goal");
        }
	};
}
