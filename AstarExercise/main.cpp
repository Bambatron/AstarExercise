#pragma once

#include <fstream>
#include "json.hpp"

#include "Map.h"

class Level {
public:
    Level();

    void Render(sf::RenderWindow& target);

    void ToggleVisualGrid() { _showVisualGrid = !_showVisualGrid; }

private:
    Map map;
    WeightedHexGrid grid;

    bool _showVisualGrid;
};

Level::Level() : map(), grid() {
    _showVisualGrid = false;
}

void Level::Render(sf::RenderWindow& target) {
    map.Render(grid, target);

    if (_showVisualGrid) {
        map.RenderVisualGrid(target);
    }    
}



int main() {
	std::cout << "Hello world!" << std::endl;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Hexagon Example", sf::Style::Default, settings);
    
    Level lv;

    // Main loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {  //Check which mouse button is pressed
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::G) {
                    lv.ToggleVisualGrid();
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        // Clear the window
        window.clear();

        // Draw the hexagon
        lv.Render(window);
        // Display the contents of the window
        window.display();
    }

	return 0;
}