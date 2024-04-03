#pragma once

#include "../Grids/HexGrid.h"

#include "GridPainter.h"

class HexPainter : public GridPainter<HexGrid> {
public:
    HexPainter(int tileSize = 30, sf::Vector2u _windowSize = sf::Vector2u(1024, 768), unsigned int maxZoomFactor = 4, unsigned int minZoomFactor = 40);

    void Render(const HexGrid& grid, sf::RenderWindow& target) override;
    void RenderSearchRecord(const HexGrid& grid, const SearchRecord<HexGrid>& record, sf::RenderWindow& target) override;
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
    if (!font.loadFromFile("../Common/wowsers.ttf")) {    //Error
        std::cout << "HexPainter::Render: Error loading map font" << std::endl;
    }
    sf::Text text;
    text.setFont(font);
    float textSize = tile.Radius() / 2.;
    text.setOrigin(textSize / 2., textSize / 2.);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(textSize);

    if (flag(PainterFlags::Node_Weights)) {
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

    if (flag(PainterFlags::Node_Center)) {
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

    if (flag(PainterFlags::Node_Coordinates)) {
        sf::Text coordText;
        coordText.setFont(font);
        float coordTextSize = tile.Radius() / 4.;
        coordText.setOrigin(coordTextSize / 2., coordTextSize / 2.);
        coordText.setFillColor(sf::Color::White);
        coordText.setCharacterSize(coordTextSize);

        for (const auto& it : grid.VisitNodes()) {
            sf::Vector2f pos;

            pos = HexToPixel(it.first, tile.Radius(), windowCenter);
            //drawNodeCoordinates(it, target);
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

void HexPainter::RenderSearchRecord(const HexGrid& grid, const SearchRecord<HexGrid>& record, sf::RenderWindow& target) {
    sf::Font font;
    if (!font.loadFromFile("../Common/wowsers.ttf")) {    //Error
        std::cout << "HexPainter::RenderSearchRecord :Error loading map font" << std::endl;
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

    if (flag(PainterFlags::Visited)) {    //Visited node
        for (auto it : record.visited) {
            circle.setFillColor(sf::Color{0, 255, 255, 196}); //Light blue
            sf::Vector2f pos = HexToPixel(it.first, tile.Radius(), windowCenter);
            circle.setPosition(pos);
            target.draw(circle);

            if (flag(PainterFlags::Costs)) {
                text.setPosition(pos);
                text.setString(std::to_string(it.second));
                target.draw(text);
            }
        }
    }

    if (flag(PainterFlags::Discovered)) { //To be visited
        circle.setFillColor(sf::Color{0, 255, 0, 196});
        for (auto it : record.discovered) {
            sf::Vector2f pos = HexToPixel(it.first, tile.Radius(), windowCenter);
            circle.setPosition(pos);
            target.draw(circle);
            
            if (flag(PainterFlags::Costs)) {
                text.setPosition(pos);
                text.setString(std::to_string(it.second));
                target.draw(text);
            }
        }
       
    }

    if (flag(PainterFlags::Path_Taken) || (!flag(PainterFlags::Path_Taken) && record.completed)) {
        for (int i = 1; i < record.pathToThisPoint.size(); i++) {
            sf::Vector2f posCurrent = HexToPixel(record.pathToThisPoint.at(i), tile.Radius(), windowCenter);
            sf::Vector2f posPrecedent = HexToPixel(record.pathToThisPoint.at(i-1), tile.Radius(), windowCenter);
            target.draw(Arrow::makeBasicArrow(posPrecedent, posCurrent).getBody());
        }
    }

    circle.setFillColor(sf::Color::Red);
    sf::Vector2f pos = HexToPixel(record.currentNode, tile.Radius(), windowCenter);
    circle.setPosition(pos);
    target.draw(circle);
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
