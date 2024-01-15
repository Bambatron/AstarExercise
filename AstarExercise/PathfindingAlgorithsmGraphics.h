#pragma once

#include <SFML/Graphics.hpp>

#include "PathfindingAlgorithms.h"

template<typename Map, typename Location>
void VisualBreadthFirstSearch(Map& map, Location& start, sf::RenderWindow& targetWindow, sf::Time frameRate = sf::seconds(1.f/60.f)) {
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
			//std::cout << "Waiting: " << elapsedTime.asSeconds() << std::endl;
		}
		
	}
}