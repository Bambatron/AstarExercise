#pragma once

#include <SFML/Graphics.hpp>

#include "HexGrid.h"

struct HexTile {
    sf::CircleShape body;

    HexTile(float radius) : body(radius, 6) {
        body.setFillColor(sf::Color::Transparent);
        body.setOutlineColor(sf::Color::Black);
        body.setOutlineThickness(1);
        body.setOrigin(radius, radius);
    }

    void SetPosition(sf::Vector2f pos) { body.setPosition(pos); }
    void SetRadius(unsigned int radius) {
        body.setOrigin(-body.getRadius(), -body.getRadius());   //Reset the origin to the top left corner
        body.setRadius(radius); //Update the radius 
        body.setOrigin(body.getRadius(), body.getRadius());   //Set the origin to the new center 
    }
    void SetFillColor(const sf::Color& color) { body.setFillColor(color); }
    void SetOutlineColor(const sf::Color& color) { body.setOutlineColor(color); }
    void SetOutlineThickness(float size) { body.setOutlineThickness(size); }

    float Radius() { return body.getRadius(); }
    float Apothem() { return ((body.getRadius() * sqrt(3.)) / 2.); }
    sf::Vector2f Position() { return body.getPosition(); }

    sf::Vector2f TopLeftSide() {
        float a = Apothem();
        //Should be (-a/2, -a sqrt(3)/2) however it goes outside the tile
        return sf::Vector2f(
            (-a / 2.) + 2.,
            (-a * sqrt(2) / 2.) + 5.);
    }
    sf::Vector2f DownLeftSide() {
        float a = Apothem();
        //Should be (-a/2, a sqrt(3)/2) however it goes outside the tile
        return sf::Vector2f(
            (-a / 2.) + 2.,
            (a * sqrt(2) / 2.) - 5.);
    }
    sf::Vector2f RightSide() {
        float a = Apothem();
        //Should be (a, 0) however it goes outside the tile
        return sf::Vector2f((a * sqrt(2) / 2.) - 2., 0);
    }
};

//template<Grid, Location, Tile>
class HexPainter {
public:
    HexPainter(sf::Vector2u _windowSize = sf::Vector2u(1024, 768), int tileSize = 30) : windowSize(_windowSize), tile(tileSize) {
        _showVisualGrid = false;
        _showHexCenter = false;
        _showHexCoordinates = false;
    }
    ~HexPainter() {}

    void Render(HexGrid& grid, sf::RenderWindow& target);

    void ToggleVisualGrid() { _showVisualGrid = !(_showVisualGrid); }
    void ToggleHexCenter() { _showHexCenter = !(_showHexCenter); }
    void ToggleHexCoordinates() { _showHexCoordinates = !(_showHexCoordinates); }
    const bool IsShowingVisualGrid() { return _showVisualGrid; }
    const bool IsShowingHexCenter() { return _showHexCenter; }
    const bool IsShowingHexCoordinates() { return _showHexCoordinates; }

    //Changes the radius of the tile by factor to aminimum of min(window height, window width) / 20
    void Zoom(float factor) {   
        tile.SetRadius((tile.Radius() + factor > std::min(windowSize.x, windowSize.y) / 20) ?
            tile.Radius() + factor :
            std::min(windowSize.x, windowSize.y) / 20);
    }

    HexTile& GetTile() { return tile; }

private:
    sf::Vector2u windowSize;

    bool _showVisualGrid;
    bool _showHexCenter;
    bool _showHexCoordinates;

    HexTile tile;
};

void HexPainter::Render(HexGrid& grid, sf::RenderWindow& target) {

}

