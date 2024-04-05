#include "pch.h"
#include "CppUnitTest.h"

#include "../Common/json.hpp"

#include "../AstarExercise/Algoritms/Dijkstra.h"
#include "../AstarExercise/Grids/HexGrid.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DijkstraUnitTest
{
	TEST_CLASS(DijkstraUnitTest)
	{
	public:
        TEST_METHOD(ValidPath) {
            Logger::WriteMessage("Dijkstra Test: starting valid path test\n");
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
                }} });

            typedef typename HexGrid::location_t location;
            typedef typename HexGrid::cost_t cost;

            //Make necessiteies for the search
            location start(0, 0);
            location goal(-1, -1);
            
            cost expectedCost = 8;

            std::vector<SearchRecord<HexGrid>> searchResult;
            searchResult = DijkstraSearchRecorded(graph, start, goal);

            // Process path
            SearchRecord<HexGrid> lastRecord = searchResult[searchResult.size() - 1];
            cost finalCost;
            for (const auto& vs : lastRecord.visited) {
                if (vs.first == goal) {
                    finalCost = vs.second;
                }
            }

            Assert::AreEqual(finalCost, expectedCost);
            Logger::WriteMessage("Dijkstra Test: valid path is passed");
        }

        TEST_METHOD(ImpossiblePath) {
            Logger::WriteMessage("Dijkstra Test: starting impossible path test\n");
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
            searchResult = DijkstraSearchRecorded(graph, start, goal);

            // Test
            for (const auto& r : searchResult) {
                Assert::AreEqual(r.completed, false);  // Checks that the search was never completed
            }
            Logger::WriteMessage("Dijkstra Test: Algortim has not found an impossible path and stopped");
        }
        TEST_METHOD(OutOfBoundStart) {
            Logger::WriteMessage("Dijkstra Test: starting out of bound startt test\n");
            HexGrid graph(2, Hex(0, 0), true);
            typedef typename HexGrid::location_t location;
            typedef typename HexGrid::cost_t cost;

            location start(3, 0);
            location goal(0, 0);
            std::vector<location> pathTaken;

            Assert::ExpectException<std::runtime_error>([&]() { DijkstraSearchRecorded(graph, start, goal); });
            Logger::WriteMessage("Dijkstra Test: invalid path is passed");
        }

        TEST_METHOD(OutOfBoundGoal) {
            Logger::WriteMessage("Dijkstra Test: starting out if bound goal test\n");
            HexGrid graph(2, Hex(0, 0), true);
            typedef typename HexGrid::location_t location;
            typedef typename HexGrid::cost_t cost;

            location start(0, 0);
            location goal(-3, 0);
            std::vector<location> pathTaken;

            Assert::ExpectException<std::runtime_error>([&]() { DijkstraSearchRecorded(graph, start, goal); });
            Logger::WriteMessage("Dijkstra Test: invalid path is passed");
        }

        TEST_METHOD(ExpensivePath) {
            Logger::WriteMessage("Dijkstra Test: starting expensive path test\n");
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
            searchResult = DijkstraSearchRecorded(graph, start, goal, 50);

            // Test
            for (const auto& r : searchResult) {
                Assert::AreEqual(r.completed, false);  // Checks that the search was never completed
            }
            Logger::WriteMessage("Dijkstra Test: Algortim has not found a path cheap eanough to the goal");
        }
	};
}
