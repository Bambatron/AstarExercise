#pragma once

#include <SFML/Graphics.hpp>

#include "PathfindingAlgorithms.h"

template<typename Graph, typename Location, typename Map>
void BFSVisual(Graph& graph, Location& start, Map& map, sf::RenderWindow& targetWindow, sf::Time frameRate = sf::seconds(1.f/60.f)) {
	auto graph = map.GetGrid();

	std::queue<Location> frontier;
	frontier.push(start);

	std::set<Location> reached;
	reached.insert(start);

	sf::CircleShape circle(map.GetTile().Radius()/5.f);
	circle.setOutlineColor(sf::Color::Black);
	circle.setOutlineThickness(1.0);
	circle.setOrigin(map.GetTile().Radius() / 5.f, map.GetTile().Radius() / 5.f);

	sf::Clock clock;
	sf::Time elapsedTime = sf::Time::Zero;

	while (!frontier.empty()) {	//Continue the search until all locations are visited
		//Restart clock and elapsed time
		clock.restart();
		elapsedTime = sf::Time::Zero;
								
		// Pop the current location from the front of the queue
		Location current = frontier.front();
		frontier.pop();

		current.Read();	 // Visualize the current location

		// For each neighbor if it has not been reached before
		for (Location next : graph.Neighbors(current)) {
			if (reached.find(next) == reached.end()) {
				// Add the neighbor to the queue and mark it as reached
				frontier.push(next);
				reached.insert(next);
				circle.setFillColor(sf::Color::Magenta);
				circle.setPosition(map.HexToPixel(next));
				targetWindow.draw(circle);
				
			}
		}
		
		targetWindow.display();

		elapsedTime += clock.restart();
		std::cout << "Elapsed time: " << elapsedTime.asSeconds() << std::endl;
		while (elapsedTime <= sf::seconds(1.f)){
			elapsedTime += clock.restart();
		}
		
	}
}

template<typename Graph, typename Location>
bool RecursiveBFSVisual(Graph& graph, std::queue<Location>& frontier, std::set<Location>& reached) {

	if (frontier.empty()) {
		return false;
	}

	// Dequeue front node and print it
	Location current = frontier.front();
	frontier.pop();

	current.Read();

	// do for every edge (v, u)
	for (Location next : graph.Neighbors(current)) {
		if (reached.find(next) == reached.end()) { // Mark neighbor as reached and enqueue it
			frontier.push(next);
			reached.insert(next);
		}
	}
	return true;
	//RecursiveBFS(graph, frontier, reached);
}

template<typename Graph, typename Location, typename Map>
std::unordered_map<Location, Location>  BFSToGoalVisual(Graph& graph, Location& start, Location& goal, Map& map, sf::RenderWindow& targetWindow, sf::Time frameRate = sf::seconds(1.f / 60.f)) {
	std::queue<Location> frontier;
	frontier.push(start);

	std::unordered_map<Location, Location> cameFrom;
	cameFrom[start] = start;

	sf::CircleShape circle(map.GetTile().Radius() / 5.f);
	circle.setOutlineColor(sf::Color::Black);
	circle.setOutlineThickness(1.0);
	circle.setOrigin(map.GetTile().Radius() / 5.f, map.GetTile().Radius() / 5.f);

	sf::Clock clock;
	sf::Time elapsedTime = sf::Time::Zero;

	while (!frontier.empty()) {	//Continue the search until all locations are visited
		//Restart clock and elapsed time
		clock.restart();
		elapsedTime = sf::Time::Zero;

		// Pop the current location from the front of the queue
		Location current = frontier.front();
		frontier.pop();

		current.Read();	 // Visualize the current location

		if (current == goal) {
			break;
		}

		targetWindow.clear();

		map.Render(graph, targetWindow);

		// For each neighbor if it has not been reached before
		for (Location next : graph.Neighbors(current)) {
			if (cameFrom.find(next) == cameFrom.end()) {
				// Add the neighbor to the queue and mark it as reached
				frontier.push(next);
				cameFrom[next] = current;
				circle.setFillColor(sf::Color::Magenta);
				circle.setPosition(map.HexToPixel(next));
				targetWindow.draw(circle);

			}
		}
		// Create a temporary copy of the queue
		std::queue<Hex> tempFrontier = frontier;
		// Visit elements in the temporary copy
		while (!tempFrontier.empty()) { //Render discovered and open nodes
			circle.setPosition(map.HexToPixel(tempFrontier.front()));
			circle.setFillColor(sf::Color::Blue);
			targetWindow.draw(circle);
			tempFrontier.pop();
		}

		targetWindow.display();

		elapsedTime += clock.restart();
		std::cout << "Elapsed time: " << elapsedTime.asSeconds() << std::endl;
		while (elapsedTime <= frameRate) {
			elapsedTime += clock.restart();
		}
	}

	return cameFrom;
}

template <typename Graph, typename Location>
bool RecursiveBFSToGoalVisual(Graph& graph, Location& goal, std::queue<Location>& frontier, std::unordered_map<Location, Location>& cameFrom) {
	if (frontier.empty()) {
		return false;
	}

	// Dequeue front node and print it
	Location current = frontier.front();
	frontier.pop();

	if (current == goal) {
		return false;
	}

	// do for every edge (v, u)
	for (Location next : graph.Neighbors(current)) {
		if (cameFrom.find(next) == cameFrom.end()) {
			// Add the neighbor to the queue and mark it as reached
			frontier.push(next);
			cameFrom[next] = current;
		}
	}
	
	return true;
}

template<typename Graph, typename Location>
bool RecursiveAstarSearchVisual(Graph graph, Location goal, PriorityQueue<Location, int>& frontier, std::unordered_map<Location, Location>& cameFrom, std::unordered_map<Location, int>& costSoFar) {
	if (frontier.isEmpty()) {
		return true;
	}

	Location current = frontier.get();

	if (current == goal) {
		return true;
	}

	for (Location next : graph.Neighbors(current)) {
		int newCost = costSoFar[current] + graph.Cost(current, next);
		if (costSoFar.find(next) == costSoFar.end() ||
			newCost < costSoFar[next]) {
			costSoFar[next] = newCost;
			int priority = newCost + Heuristic(next, goal);
			frontier.put(next, priority);
			cameFrom[next] = current;
		}
	}

	return false;
}
