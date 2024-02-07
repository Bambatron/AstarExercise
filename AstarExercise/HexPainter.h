#pragma once

#include "HexGrid.h"

#include "PainterInterface.h"

struct HexTile {
    sf::CircleShape body;

    HexTile(float radius) : body(radius, 6) {
        body.setFillColor(sf::Color::Transparent);
        body.setOutlineColor(sf::Color::Black);
        body.setOutlineThickness(1);
        body.setOrigin(radius, radius);
    }
    HexTile(const HexTile& other) : body(other.body) {}
    
    void SetOrigin(sf::Vector2f origin) { body.setOrigin(origin); }
    void SetPosition(sf::Vector2f pos) { body.setPosition(pos); }
    void SetFillColor(const sf::Color& color) { body.setFillColor(color); }
    void SetOutlineColor(const sf::Color& color) { body.setOutlineColor(color); }
    void SetOutlineThickness(float size) { body.setOutlineThickness(size); }
    sf::Vector2f Position() { return body.getPosition(); }


    void SetRadius(unsigned int radius) {
        body.setOrigin(-body.getRadius(), -body.getRadius());   //Reset the origin to the top left corner
        body.setRadius(radius); //Update the radius 
        body.setOrigin(body.getRadius(), body.getRadius());   //Set the origin to the new center 
    }
    float Radius() { return body.getRadius(); }
    float Apothem() { return ((body.getRadius() * sqrt(3.)) / 2.); }
    
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
class HexPainter : public Painter<HexTile> {
public:
    HexPainter(int tileSize = 30, sf::Vector2u _windowSize = sf::Vector2u(1024, 768), unsigned int maxZoomFactor = 4, unsigned int minZoomFactor = 40);

    void Render(HexGrid& grid, sf::RenderWindow& target) override;

    //Changes the radius of the tile by factor to aminimum of min(window height, window width) / 20
    void Zoom(float factor) override;
};

HexPainter::HexPainter(int tileSize, sf::Vector2u _windowSize, unsigned int maxZoomFactor, unsigned int minZoomFactor) : Painter<HexTile>(HexTile(tileSize), _windowSize, maxZoomFactor, minZoomFactor) {}

void HexPainter::Render(HexGrid& grid, sf::RenderWindow& target) {
    //Draw background hexes map zoomed in as in camera
    for (const auto& it : grid.VisitNodes()) {
        sf::Vector2f pos = HexToPixel(it.first, tile.Radius(), windowCenter);
        tile.SetPosition(pos);
        tile.SetFillColor(sf::Color::Black);
        tile.SetOutlineColor(sf::Color::White);
        tile.SetOutlineThickness(3);
        target.draw(tile.body);
    }

    //Resetting view to graw non blurry text
    sf::Font font;
    if (!font.loadFromFile("wowsers.ttf")) {    //Error
        std::cout << "Error loading map font" << std::endl;
    }
    sf::Text text;
    text.setFont(font);
    float textSize = tile.Radius() / 2.;
    text.setOrigin(textSize / 2., textSize / 2.);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(textSize);

    for (const auto& it : grid.VisitNodes()) {
        sf::Vector2f pos = HexToPixel(it.first, tile.Radius(), windowCenter);
        text.setPosition(pos);
        std::string tmp = std::to_string(it.second);
        text.setString(tmp);
        target.draw(text);
    }

    tile.SetOutlineColor(sf::Color::Black);
    tile.SetOutlineThickness(1);

    if (_showHexCenter) {
        float size = tile.Radius() / 5.;
        sf::CircleShape circleTile;
        circleTile.setRadius(size);
        circleTile.setFillColor(sf::Color::Yellow);
        circleTile.setOrigin(size / 2., size / 2.);

        for (const auto& it : grid.VisitNodes()) {
            sf::Vector2f pos;
            pos = HexToPixel(it.first, tile.Radius(), windowCenter);

            circleTile.setPosition(pos);
            target.draw(circleTile);
        }
    }
    if (_showHexCoordinates) {
        sf::Text coordText;
        coordText.setFont(font);
        float coordTextSize = tile.Radius() / 4.;
        coordText.setOrigin(coordTextSize / 2., coordTextSize / 2.);
        coordText.setFillColor(sf::Color::White);
        coordText.setCharacterSize(coordTextSize);

        for (const auto& it : grid.VisitNodes()) {
            sf::Vector2f pos;

            pos = HexToPixel(it.first, tile.Radius(), windowCenter);

            text.setFillColor(sf::Color::Yellow);
            text.setPosition(pos + tile.TopLeftSide());
            std::string tmp = std::to_string(it.first.q);
            text.setString(tmp);
            target.draw(text);

            text.setFillColor(sf::Color::Green);
            text.setPosition(pos + tile.RightSide());
            tmp = std::to_string(it.first.r);
            text.setString(tmp);
            target.draw(text);

            text.setFillColor(sf::Color::Red);
            text.setPosition(pos + tile.DownLeftSide());
            tmp = std::to_string(it.first.s);
            text.setString(tmp);
            target.draw(text);
        }
    }
}

void HexPainter::Zoom(float factor) {
    unsigned int rad = tile.Radius() + factor;
    
    if (rad > maxZoom) {
        rad = maxZoom;
    }
    else if (rad < minZoom) {
        rad = minZoom;
    }

    tile.SetRadius(rad);
}
