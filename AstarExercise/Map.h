#pragma once

#include <SFML/Graphics.hpp>

#include "WeightedSquareGrid.h"
#include "WeightedHexagonalGrid.h"
#include "PathfindingAlgorithsmGraphics.h"

struct HexTile {
    HexTile(float radius) : body(radius, 6) {
        body.setFillColor(sf::Color::Transparent);
        body.setOutlineColor(sf::Color::Black);
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

template<typename Grid>
class Map {
public:
    Map(int tileSize = 30, sf::Vector2i _windowSize = sf::Vector2i(800, 600));

    void Render(const Grid& grid, sf::RenderWindow& target);
    void RenderVisualGrid(sf::RenderWindow& target);

    sf::Vector2i GetWindowSize() { return windowSize; }
    HexTile& GetTile() { return tile; }

    sf::Vector2f HexToPixel(const Hex& hex);
    Hex PixelToHex(sf::Vector2i pixelPos);

private:
    sf::Vector2i windowSize;

    HexTile tile;
};

template<typename Grid>
Map<Grid>::Map(int tileSize, sf::Vector2i _windowSize) : tile(tileSize), windowSize(_windowSize) {
   
}

template<typename Grid>
void Map<Grid>::RenderVisualGrid(sf::RenderWindow& target) {
    //This grid is cetered on the middle of the window
    for (int i = 0; i < windowSize.x / 2; i += tile.Radius() * sqrt(3) / 2) {
        sf::VertexArray verticalRight(sf::LinesStrip, 2);
        verticalRight[0].position = sf::Vector2f(windowSize.x / 2 + i, 0);
        verticalRight[0].color = sf::Color::White;
        verticalRight[1].position = sf::Vector2f(windowSize.x / 2 + i, windowSize.y);
        verticalRight[1].color = sf::Color::White;
        sf::VertexArray verticalLeft(sf::LinesStrip, 2);
        verticalLeft[0].position = sf::Vector2f(windowSize.x / 2 - i, 0);
        verticalLeft[0].color = sf::Color::White;
        verticalLeft[1].position = sf::Vector2f(windowSize.x / 2 - i, windowSize.y);
        verticalLeft[1].color = sf::Color::White;
        target.draw(verticalRight);
        target.draw(verticalLeft);
    }
    for (int i = 0; i < windowSize.y / 2; i += tile.Radius()) {
        sf::VertexArray horizontalDown(sf::LinesStrip, 2);
        horizontalDown[0].position = sf::Vector2f(0, windowSize.y / 2 + i);
        horizontalDown[0].color = sf::Color::White;
        horizontalDown[1].position = sf::Vector2f(windowSize.x, windowSize.y / 2 + i);
        horizontalDown[1].color = sf::Color::White;
        sf::VertexArray horizontalUp(sf::LinesStrip, 2);
        horizontalUp[0].position = sf::Vector2f(0, windowSize.y / 2 - i);
        horizontalUp[0].color = sf::Color::White;
        horizontalUp[1].position = sf::Vector2f(windowSize.x, windowSize.y / 2 - i);
        horizontalUp[1].color = sf::Color::White;
        target.draw(horizontalDown);
        target.draw(horizontalUp);
    }
}

template <typename Grid>
sf::Vector2f Map<Grid>::HexToPixel(const Hex& hex) {
    float x, y;
    int size = tile.Radius();

    x = size * (sqrt(3) * hex.q + sqrt(3) / 2 * hex.r);
    y = size * (3. / 2 * hex.r);

    x += windowSize.x / 2;
    y += windowSize.y / 2;

    return sf::Vector2f(x, y);
}

template <typename Grid>
Hex Map<Grid>::PixelToHex(sf::Vector2i pixelPos) {
    pixelPos.x -= windowSize.x / 2;
    pixelPos.y -= windowSize.y / 2;

    int radius = tile.Radius();

    double q = ((pixelPos.x * sqrt(3) / 3) - (pixelPos.y * 1. / 3)) / radius;
    double r = (pixelPos.y * 2. * 1. / 3) / radius;

    return HexRound(q, r, -q - r);
}

void Map<ForestHexGrid>::Render(const ForestHexGrid& grid, sf::RenderWindow& target) {
    for (const auto& it : grid.VisitNodes()) {
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

void Map<WeightedHexGrid>::Render(const WeightedHexGrid& grid, sf::RenderWindow& target) {
    sf::Font font;
    if (!font.loadFromFile("wowsers.ttf")) {
        std::cout << "Error loading map font" << std::endl;
    } //Error
    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(tile.Radius() / 2);
    
    for (const auto& it : grid.VisitNodes()) {
        sf::Vector2f pos = HexToPixel(it.first);
        tile.SetPosition(pos);
        tile.SetFillColor(sf::Color::Black);
        tile.SetOutlineColor(sf::Color::White);
        tile.SetOutlineThickness(3);

        std::string tmp = std::to_string(it.second);
        text.setString(tmp);
        text.setPosition(pos);

        target.draw(tile.body);
        target.draw(text);
    }

    tile.SetOutlineColor(sf::Color::Black);
    tile.SetOutlineThickness(1);
}