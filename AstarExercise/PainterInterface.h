#pragma once

#include <SFML/Graphics.hpp>

template<typename Graph>
class Painter {
public:
    using Tile = typename Graph::Tile;

    Painter(const Tile& initialTile, sf::Vector2u _windowSize = sf::Vector2u(1024, 768), unsigned int maxZoomFactor = 4, unsigned int minZoomFactor = 40);
    virtual ~Painter() {}

	virtual void Render(Graph& grid, sf::RenderWindow& target) = 0;

    void ToggleVisualGrid() { _showVisualGrid = !(_showVisualGrid); }
    void ToggleHexCenter() { _showHexCenter = !(_showHexCenter); }
    void ToggleHexCoordinates() { _showHexCoordinates = !(_showHexCoordinates); }
    const bool IsShowingVisualGrid() { return _showVisualGrid; }
    const bool IsShowingHexCenter() { return _showHexCenter; }
    const bool IsShowingHexCoordinates() { return _showHexCoordinates; }

    virtual void Zoom(float factor) = 0;
    void MoveCamera(sf::Vector2i offset) { windowCenter += offset; }
    void MoveCamera(int offsetX, int offsetY) { MoveCamera(sf::Vector2i(offsetX, offsetY)); }

    void SetMaxZoom(unsigned int maxZoomFactor) { maxZoom = std::min(windowSize.x, windowSize.y) / maxZoomFactor; }
    void SetMinZoom(unsigned int minZoomFactor) { minZoom = std::min(windowSize.x, windowSize.y) / minZoomFactor; }

    const sf::Vector2i GetWindowCenter() { return windowCenter; }

    Tile& GetTile() { return tile; }

protected:
    sf::Vector2u windowSize;
    sf::Vector2i windowCenter;

    unsigned int maxZoom, minZoom;

    bool _showVisualGrid;
    bool _showHexCenter;
    bool _showHexCoordinates;

	Tile tile;
};

template<typename Graph>
Painter<Graph>::Painter(const typename Graph::Tile& initialTile, sf::Vector2u _windowSize, unsigned int maxZoomFactor, unsigned int minZoomFactor) : tile(initialTile), windowSize(_windowSize) {
    windowCenter = sf::Vector2i(_windowSize.x / 2, _windowSize.y / 2);

    SetMaxZoom(maxZoomFactor);
    SetMinZoom(minZoomFactor);

    _showVisualGrid = false;
    _showHexCenter = false;
    _showHexCoordinates = false;
}
