#pragma once

#include "../Grids/HexGrid.h"

#include "GridPainter.h"

class HexPainter : public GridPainter<HexGrid> {
public:
    HexPainter(int tileSize = 30, sf::Vector2u _windowSize = sf::Vector2u(1024, 768), unsigned int maxZoomFactor = 4, unsigned int minZoomFactor = 40);

    void Render(const HexGrid& grid, sf::RenderWindow& target) override;
    void RenderSearchRecord(const HexGrid& grid, const SearchRecord<typename HexGrid::Location>& record, sf::RenderWindow& target) override;
    //Changes the radius of the tile by factor to aminimum of min(window height, window width) / 20
    void Zoom(float factor) override;
};

HexPainter::HexPainter(int tileSize, sf::Vector2u _windowSize, unsigned int maxZoomFactor, unsigned int minZoomFactor) : GridPainter<HexGrid>(HexTile(tileSize), _windowSize, maxZoomFactor, minZoomFactor) {}

void HexPainter::Render(const HexGrid& grid, sf::RenderWindow& target) {
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

    if (_showNodeWeights) {
        for (const auto& it : grid.VisitNodes()) {
            sf::Vector2f pos = HexToPixel(it.first, tile.Radius(), windowCenter);
            text.setPosition(pos);
            std::string tmp = std::to_string(it.second);
            text.setString(tmp);
            target.draw(text);
        }
    }

    tile.SetOutlineColor(sf::Color::Black);
    tile.SetOutlineThickness(1);

    if (_showNodeCenter) {
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

    if (_showNodeCoordinates) {
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

void HexPainter::RenderSearchRecord(const HexGrid& grid, const SearchRecord<typename HexGrid::Location>& record, sf::RenderWindow& target) {
    sf::Font font;
    if (!font.loadFromFile("wowsers.ttf")) {    //Error
        std::cout << "Error loading map font" << std::endl;
    }
    sf::Text text;
    text.setFont(font);
    float textSize = tile.Radius() / 2.;
    text.setOrigin(textSize / 2., textSize / 2.);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(0.5);
    text.setCharacterSize(textSize);

    float rad = tile.Radius() / 3.;
    sf::CircleShape circle(rad);
    circle.setOrigin(rad, rad);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(1.5);

    if (_showRecordedVisiteds) {
        for (auto it : record.visited) {
            //Discovered nodes
            circle.setFillColor(sf::Color{ 255, 0, 255, 128 }); //Fucsia
            sf::Vector2f pos = HexToPixel(it.first, tile.Radius(), windowCenter);
            circle.setPosition(pos);
            target.draw(circle);

            //Currently visited nodes
            circle.setFillColor(sf::Color{ 0, 255, 255, 128 }); //Light blue
            pos = HexToPixel(it.second, tile.Radius(), windowCenter);
            circle.setPosition(pos);
            target.draw(circle);
        }
    }

    if (_showRecordedDiscovereds) { //To be visited
        circle.setFillColor(sf::Color{ 0,255,0,128 });
        for (auto it : record.discovered) {
            sf::Vector2f pos = HexToPixel(it, tile.Radius(), windowCenter);
            circle.setPosition(pos);
            target.draw(circle);
        }
    }

    circle.setFillColor(sf::Color::Red);
    sf::Vector2f pos = HexToPixel(record.currentNode, tile.Radius(), windowCenter);
    circle.setPosition(pos);
    target.draw(circle);

    //All visited and discovered with cost
    if (_showRecordedCosts) {
        circle.setFillColor(sf::Color{ 255, 255, 0, 128 });
        for (auto it : record.costSoFar) {
            sf::Vector2f pos = HexToPixel(it.first, tile.Radius(), windowCenter);
            circle.setPosition(pos);
            target.draw(circle);
            text.setPosition(pos);
            text.setString(std::to_string(it.second));
            target.draw(text);
        }
    }
}

void HexPainter::Zoom(float factor) {
    unsigned int rad = tile.Radius() + factor;  //Radius of zoomed tile
    
    if (rad > maxZoom) {    //Checks if too big
        rad = maxZoom; 
    }
    else if (rad < minZoom) {   //Checks if too small
        rad = minZoom;
    }

    tile.SetRadius(rad);    //Zooms by enlarging or reducing the size of the tile    
    //Text loses details when zoomed
    //This is a lot easier than having two view of the map one for the zoomed tiles and one for the text
}