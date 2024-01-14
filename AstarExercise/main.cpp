#pragma once

#include <fstream>
#include "json.hpp"

#include <SFML/Graphics.hpp>

#include "WeightedSquareGrid.h"
#include "WeightedHexagonalGrid.h"
#include "PathfindingAlgorithms.h"

struct HexTile {
    HexTile(float radius) : body(radius, 6) {
        body.setFillColor(sf::Color::Transparent);
        body.setOutlineColor(sf::Color::White);
        body.setOutlineThickness(1);
        body.setOrigin(sf::Vector2f(radius, radius));
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
    Map(int tileSize = 30, sf::Vector2i _windowSize = sf::Vector2i(800,600), const std::string& mapName = "example.json");

	void Render(sf::RenderWindow& target);

    sf::Vector2f HexToPixel(const Hex& hex);
    Hex PixelToHex(sf::Vector2i pixelPos);

private:
    sf::Vector2i windowSize;

    TileType tile;
	GridType grid;
};

template<typename TileType, typename GridType>
Map<TileType, GridType>::Map(int tileSize, sf::Vector2i _windowSize, const std::string& mapName) : tile(TileType(tileSize)), grid(GridType(mapName)), windowSize(_windowSize)  {
}

template<typename TileType, typename GridType>
void Map<TileType, GridType>::Render(sf::RenderWindow& target) {
    for (auto it : grid.VisitNodes()) {
        sf::Vector2f pos = HexToPixel(it);
        tile.SetPosition(pos);
        
        if (grid.IsForest(it)) {
            tile.SetFillColor(sf::Color::Green);
        }
        else {
            tile.SetFillColor(sf::Color::Yellow);
        }

        target.draw(tile.body);
    }   
}

template<typename TileType, typename GridType>
sf::Vector2f Map<TileType, GridType>::HexToPixel(const Hex& hex) {
    float x, y;
    int size = tile.Radius();
    
    x = size * (sqrt(3) * hex.q + sqrt(3) / 2 * hex.r);
    y = size * (3. / 2 * hex.r);
    
    x += windowSize.x / 2;
    y += windowSize.y / 2;

    return sf::Vector2f(x, y);
}

template<typename TileType, typename GridType>
Hex Map<TileType, GridType>::PixelToHex (sf::Vector2i pixelPos) {
    pixelPos.x -= windowSize.x/2;
    pixelPos.y -= windowSize.y/2;
    
    int radius = tile.Radius();

    double q = ((pixelPos.x * sqrt(3) / 3) - (pixelPos.y *1. / 3)) / radius;
    double r = (pixelPos.y * 2. * 1./ 3) / radius;
    return HexRound(q, r, -q-r);
}

int main() {
	std::cout << "Hello world!" << std::endl;
	
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Hexagon Example", sf::Style::Default, settings);
    sf::Vector2i windowSize = sf::Vector2i(window.getSize().x, window.getSize().y) ;
    bool _showGrid = false;

    int rad = 50;

    Map<HexTile, WeightedHexGrid> map(rad, windowSize);

    map.PixelToHex(sf::Vector2i(369, 236));

    // Main loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {  //Check which mouse button is pressed
                if (event.mouseButton.button == sf::Mouse::Left) {  //Get the mouse position and print it
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    std::cout << "Mouse Clicked at: " << mousePosition.x << ", " << mousePosition.y << std::endl;
                    auto hex = map.PixelToHex(mousePosition);
                    std::cout << "Hex found: " << hex.q << " " << hex.r << " " << hex.s << std::endl;
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::G) {
                    _showGrid = !(_showGrid);
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        // Clear the window
        window.clear();

        // Draw the hexagon
        map.Render(window);
        if (_showGrid) {
            //This grid is cetered on the middle of the window
            for (int i = 0; i < windowSize.x /2 ; i += rad * sqrt(3) / 2) {
                sf::VertexArray verticalRight(sf::LinesStrip, 2);
                verticalRight[0].position = sf::Vector2f(windowSize.x / 2 + i, 0);
                verticalRight[0].color = sf::Color::Red;
                verticalRight[1].position = sf::Vector2f(windowSize.x / 2 + i, windowSize.y);
                verticalRight[1].color = sf::Color::Red;
                sf::VertexArray verticalLeft(sf::LinesStrip, 2);
                verticalLeft[0].position = sf::Vector2f(windowSize.x / 2 - i, 0);
                verticalLeft[0].color = sf::Color::Red;
                verticalLeft[1].position = sf::Vector2f(windowSize.x / 2 - i, windowSize.y);
                verticalLeft[1].color = sf::Color::Red;
                window.draw(verticalRight);
                window.draw(verticalLeft);
            }
            for (int i = 0; i < windowSize.y / 2; i += rad) {
                sf::VertexArray horizontalDown(sf::LinesStrip, 2);
                horizontalDown[0].position = sf::Vector2f(0, windowSize.y / 2 + i);
                horizontalDown[0].color = sf::Color::Red;
                horizontalDown[1].position = sf::Vector2f(windowSize.x, windowSize.y / 2 + i);
                horizontalDown[1].color = sf::Color::Red;
                sf::VertexArray horizontalUp(sf::LinesStrip, 2);
                horizontalUp[0].position = sf::Vector2f(0, windowSize.y / 2 - i);
                horizontalUp[0].color = sf::Color::Red;
                horizontalUp[1].position = sf::Vector2f(windowSize.x, windowSize.y / 2 - i);
                horizontalUp[1].color = sf::Color::Red;
                window.draw(horizontalDown);
                window.draw(horizontalUp);
            }
        }

        // Display the contents of the window
        window.display();
    }

	return 0;
}