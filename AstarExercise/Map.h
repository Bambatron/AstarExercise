#pragma once+

#include <SFML/Graphics.hpp>

#include "WeightedSquareGrid.h"
#include "WeightedHexagonalGrid.h"
#include "PathfindingAlgorithsmGraphics.h"

struct HexTile {
    sf::CircleShape body;

    HexTile(float radius) : body(radius, 6) {
        body.setFillColor(sf::Color::Transparent);
        body.setOutlineColor(sf::Color::Black);
        body.setOutlineThickness(1);
        body.setOrigin(sf::Vector2f(radius, radius));
    }

    void SetPosition(sf::Vector2f pos) { body.setPosition(pos); }
    void SetRadius(float radius) { body.setRadius(radius); }
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
            (-a / 2.) +2.,
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

sf::Vector2f transformText(const sf::Vector2f& position, const sf::RenderWindow& window, const sf::View& viewFrom, const sf::View& viewTo) {
    sf::Vector2i positionInWindow = window.mapCoordsToPixel(position, viewFrom);
    return window.mapPixelToCoords(positionInWindow, viewTo);
}

template<typename Grid>
class Map {
public:
    Map(sf::Vector2u _windowSize = sf::Vector2i(1024, 768), int tileSize = 30);

    void Render(const Grid& grid, sf::RenderWindow& target);
    void RenderVisualGrid(sf::RenderWindow& target);
    
    void ToggleVisualGrid() { _showVisualGrid = !(_showVisualGrid); }
    void ToggleHexCenter() { _showHexCenter = !(_showHexCenter); }
    void ToggleHexCoordinates() { _showHexCoordinates = !(_showHexCoordinates); }

    void Zoom(float factor) {}
    //void MoveCamera(sf::Vector2f offset) { camera.move(offset); }
    //void MoveCamera(float offsetX, float offsetY) { MoveCamera(sf::Vector2f(offsetX, offsetY)); }

    sf::Vector2i GetWindowSize() { return windowSize; }
    HexTile& GetTile() { return tile; }

    sf::Vector2f HexToPixel(const Hex& hex);
    Hex PixelToHex(sf::Vector2f pixelPos, sf::RenderWindow& target);

private:
    sf::Vector2i windowSize;

    /*sf::View camera;
    float zoomLevel;*/

    bool _showVisualGrid;
    bool _showHexCenter;
    bool _showHexCoordinates;

    HexTile tile;
};

template<typename Grid>
Map<Grid>::Map(sf::Vector2u _windowSize, int tileSize) :  windowSize(_windowSize), tile(tileSize) {
    _showVisualGrid = false;
    _showHexCenter = false;
    _showHexCoordinates = false;
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
Hex Map<Grid>::PixelToHex(sf::Vector2f pixelPos, sf::RenderWindow& target) {
    pixelPos.x -= windowSize.x / 2;
    pixelPos.y -= windowSize.y / 2;

    int radius = tile.Radius();

    double q = ((pixelPos.x * sqrt(3) / 3) - (pixelPos.y * 1. / 3)) / radius;
    double r = (pixelPos.y * 2. * 1. / 3) / radius;

    return HexRound(q, r, -q - r);
}

template<typename Grid>
void Map<Grid>::Render(const Grid& frid, sf::RenderWindow& target) {}

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

    if (_showHexCenter) {
        sf::CircleShape circleTile;
        circleTile.setRadius(5);
        circleTile.setFillColor(sf::Color::Yellow);
        circleTile.setOrigin(2.5, 2.5);

        for (const auto& it : grid.VisitNodes()) {
            sf::Vector2f pos;
            pos = HexToPixel(it);

            circleTile.setPosition(pos);
            target.draw(circleTile);
        }
    }
    if (_showHexCoordinates) {
        sf::Font font;
        if (!font.loadFromFile("wowsers.ttf")) {    //Error
            std::cout << "Error loading map font" << std::endl;
        }
        sf::Text text;
        text.setFont(font);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(tile.Radius() / 3);

        for (const auto& it : grid.VisitNodes()) {
            sf::Vector2f pos;
            pos = HexToPixel(it);

            text.setFillColor(sf::Color::Yellow);
            text.setPosition(pos + tile.TopLeftSide());
            std::string tmp = std::to_string(it.q);
            text.setString(tmp);
            target.draw(text);

            text.setFillColor(sf::Color::Green);
            text.setPosition(pos + tile.RightSide());
            tmp = std::to_string(it.r);
            text.setString(tmp);
            target.draw(text);

            text.setFillColor(sf::Color::Red);
            text.setPosition(pos + tile.DownLeftSide());
            tmp = std::to_string(it.s);
            text.setString(tmp);
            target.draw(text);
        }
    }
}

void Map<WeightedHexGrid>::Render(const WeightedHexGrid& grid, sf::RenderWindow& target) {
    //Draw background hexes map zoomed in as in camera
    for (const auto& it : grid.VisitNodes()) {
        sf::Vector2f pos = HexToPixel(it.first);
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
    text.setOrigin(textSize/2., textSize/2.);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(textSize);

    for (const auto& it : grid.VisitNodes()) {
        sf::Vector2f pos = HexToPixel(it.first);
        std::string tmp = std::to_string(it.second);
        text.setString(tmp);
        text.setPosition(pos);
        target.draw(text);
    }

    tile.SetOutlineColor(sf::Color::Black);
    tile.SetOutlineThickness(1);
    
    if (_showHexCenter) {
        float size = tile.Radius() / 5.;
        sf::CircleShape circleTile;
        circleTile.setRadius(size);
        circleTile.setFillColor(sf::Color::Yellow);
        circleTile.setOrigin(size/2., size/2.);

        for (const auto& it : grid.VisitNodes()) {
            sf::Vector2f pos;
            pos = HexToPixel(it.first);

            circleTile.setPosition(pos);
            target.draw(circleTile);
        }
    }    
    if (_showHexCoordinates) {
        textSize = tile.Radius() / 3.;
        text.setCharacterSize(textSize);

        for (const auto& it : grid.VisitNodes()) {
            sf::Vector2f pos;
            
            pos = HexToPixel(it.first);

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


