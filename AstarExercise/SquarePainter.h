#pragma once

#include "SquareGrid.h"

#include "PainterInterface.h"

class SquarePainter : public Painter<SquareGrid> {
public:
	SquarePainter(int tileSide = 40, sf::Vector2u _windowSize = sf::Vector2u(1024, 768), unsigned int maxZoomFactor = 4, unsigned int minZoomFactor = 40);

	void Render(SquareGrid& grid, sf::RenderWindow& target) override;

	void Zoom(float factor) override;
};

SquarePainter::SquarePainter(int tileSide, sf::Vector2u _windowSize, unsigned int maxZoomFactor, unsigned int minZoomFactor) : Painter<SquareGrid>(SquareTile(tileSide), _windowSize, maxZoomFactor, minZoomFactor) {}

void SquarePainter::Render(SquareGrid& grid, sf::RenderWindow& target) {
    for (const auto& it : grid.VisitNodes()) {
        sf::Vector2f pos = SquareToPixel(it.first, tile.Side(), windowCenter);
        tile.SetPosition(pos);
        tile.SetFillColor(sf::Color::Black);
        tile.SetOutlineColor(sf::Color::White);
        tile.SetOutlineThickness(3);
        target.draw(tile.body);
    }

    sf::Font font;
    if (!font.loadFromFile("wowsers.ttf")) {    //Error
        std::cout << "Error loading map font" << std::endl;
    }
    sf::Text text;
    text.setFont(font);
    float textSize = tile.Side() / 3.;
    text.setOrigin(textSize / 2., textSize / 2.);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(textSize);

    for (const auto& it : grid.VisitNodes()) {
        sf::Vector2f pos = SquareToPixel(it.first, tile.Side(), windowCenter);
        text.setPosition(pos);
        std::string tmp = std::to_string(it.second);
        text.setString(tmp);
        target.draw(text);
    }

    tile.SetOutlineColor(sf::Color::Black);
    tile.SetOutlineThickness(1);
    
    if (_showHexCenter) {
        float size = tile.Side() / 5.;
        sf::CircleShape circleTile;
        circleTile.setRadius(size);
        circleTile.setFillColor(sf::Color::Yellow);
        circleTile.setOrigin(size / 2., size / 2.);

        for (const auto& it : grid.VisitNodes()) {
            sf::Vector2f pos;
            pos = SquareToPixel(it.first, tile.Side(), windowCenter);

            circleTile.setPosition(pos);
            target.draw(circleTile);
        }
    }

    if (_showHexCoordinates) {
        sf::Text coordText;
        coordText.setFont(font);
        float coordTextSize = tile.Side() / 4.;
        coordText.setOrigin(coordTextSize / 2., coordTextSize / 2.);
        coordText.setFillColor(sf::Color::White);
        coordText.setCharacterSize(coordTextSize);

        for (const auto& it : grid.VisitNodes()) {
            sf::Vector2f pos = SquareToPixel(it.first, tile.Side(), windowCenter);
            
            pos += tile.TopHalfSide();
            text.setPosition(pos);
            
            text.setFillColor(sf::Color::Yellow);
            
            std::string tmp = it.first.PrintOut();
            text.setString(tmp);
            
            target.draw(text);
        }
    }
}


void SquarePainter::Zoom(float factor) {
    unsigned int side = tile.Side() + factor;  //Side of zoomed tile

    if (side > maxZoom) {    //Checks if too big
        side = maxZoom;
    }
    else if (side < minZoom) {   //Checks if too small
        side = minZoom;
    }

    tile.SetSide(side);    //Zooms by enlarging or reducing the size of the tile    
    //Text loses details when zoomed
    //This is a lot easier than having two view of the map one for the zoomed tiles and one for the text
}