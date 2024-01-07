#pragma once

#include <fstream>
#include "json.hpp"

#include <SFML/Graphics.hpp>

#include "WeightedSquareGrid.h"
#include "WeightedHexagonalGrid.h"
#include "PathfindingAlgorithms.h"

struct HexTile {
    HexTile(float radius) : body(radius, 6) {
        body.setRotation(30);
        body.setFillColor(sf::Color::Green);
        body.setOutlineColor(sf::Color::Black);
        body.setOutlineThickness(2);
    }

    sf::Vector2f Position() { return body.getPosition(); }
    float Radius() { return body.getRadius(); }

    void SetPosition(sf::Vector2f pos) { body.setPosition(pos); }
    void SetRadius(float radius) { body.setRadius(radius); }
    void SetFillColor(const sf::Color& color) { body.setFillColor(color); }
    void SetOutlineColor(const sf::Color& color) { body.setOutlineColor(color); }
    void SetOutlineThickness(float size) { body.setOutlineThickness(size); }

    sf::CircleShape body;
};

template<typename TileType, typename GridType>
class Map {
public:
    Map(int tileSize = 30, const std::string& mapName = "example.json");

	void Render(sf::RenderWindow& target);

private:
    TileType tile;
	GridType grid;
};

template<typename TileType, typename GridType>
Map<TileType, GridType>::Map(int tileSize, const std::string& mapName) : tile(TileType(tileSize)), grid(GridType(mapName))  {
	//grid->ReadGrid();
}

template<typename TileType, typename GridType>
void Map<TileType, GridType>::Render(sf::RenderWindow& target) {
    if (tile.body == nullptr) {
        std::cout << "body does not exist" << std::endl;
    }
    target.draw(*tile.body);   
}

int main() {
	std::cout << "Hello world!" << std::endl;
	
    sf::RenderWindow window(sf::VideoMode(800, 600), "Hexagon Example");

    Map<HexTile, WeightedHexGrid> map(50);

    HexTile hexagon(50);
    hexagon.SetPosition(sf::Vector2f(400, 300));

    // Main loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        std::cout << "render" << std::endl;

        // Clear the window
        window.clear();

        // Draw the hexagon
        window.draw(hexagon.body);
        //map.Render(window);

        // Display the contents of the window
        window.display();
    }

	return 0;
}