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

class Map {
public:
    Map(int tileSize = 30, sf::Vector2i _windowSize = sf::Vector2i(800, 600));

    void Render(const WeightedHexGrid& grid, sf::RenderWindow& target);
    void RenderVisualGrid(sf::RenderWindow& target);

    sf::Vector2f HexToPixel(const Hex& hex);
    Hex PixelToHex(sf::Vector2i pixelPos);

    const HexTile& GetTile() { return tile; }

private:
    sf::Vector2i windowSize;

    HexTile tile;
};

Map::Map(int tileSize, sf::Vector2i _windowSize) : tile(tileSize), windowSize(_windowSize) {}

void Map::Render(const WeightedHexGrid& grid, sf::RenderWindow& target) {
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

void Map::RenderVisualGrid(sf::RenderWindow& target) {
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

sf::Vector2f Map::HexToPixel(const Hex& hex) {
    float x, y;
    int size = tile.Radius();

    x = size * (sqrt(3) * hex.q + sqrt(3) / 2 * hex.r);
    y = size * (3. / 2 * hex.r);

    x += windowSize.x / 2;
    y += windowSize.y / 2;

    return sf::Vector2f(x, y);
}

Hex Map::PixelToHex(sf::Vector2i pixelPos) {
    pixelPos.x -= windowSize.x / 2;
    pixelPos.y -= windowSize.y / 2;

    int radius = tile.Radius();

    double q = ((pixelPos.x * sqrt(3) / 3) - (pixelPos.y * 1. / 3)) / radius;
    double r = (pixelPos.y * 2. * 1. / 3) / radius;
    return HexRound(q, r, -q - r);
}