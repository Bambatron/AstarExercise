#pragma once

#include <SFML/Graphics.hpp>

#include "../Pathfinder/Pathfinder.h"

template<typename Grid>
class GridPainter {
public:
    using Tile = typename Grid::Tile;

    GridPainter(const Tile& initialTile, sf::Vector2u _windowSize = sf::Vector2u(1024, 768), unsigned int maxZoomFactor = 4, unsigned int minZoomFactor = 40);
    virtual ~GridPainter() {}

	virtual void Render(const Grid& grid, sf::RenderWindow& target) = 0;
    virtual void RenderSearchRecord(const Grid& grid, const SearchRecord<typename Grid::Location>& record, sf::RenderWindow& target) = 0;

    void ToggleVisualGrid() { _showVisualGrid = !(_showVisualGrid); }
    void ToggleNodeCenter() { _showNodeCenter = !(_showNodeCenter); }
    void ToggleNodeCoordinates() { _showNodeCoordinates = !(_showNodeCoordinates); }
    void ToggleNodeWeights() { _showNodeWeights = !(_showNodeWeights); }
    const bool IsShowingVisualGrid() { return _showVisualGrid; }
    const bool IsShowingNodeCenter() { return _showNodeCenter; }
    const bool IsShowingNodeCoordinates() { return _showNodeCoordinates; }
    const bool IsShowingNodeWeights() { return _showNodeWeights; }

    void ToggleRecordedVisiteds() { _showRecordedVisiteds = !(_showRecordedVisiteds); }
    void ToggleRecordedCosts() { _showRecordedCosts = !(_showRecordedCosts); }
    void ToggleRecordedDiscovereds() { _showRecordedDiscovereds = !(_showRecordedDiscovereds); }
    const bool IsShowingRecordedVisiteds() { return _showRecordedVisiteds; }
    const bool IsShowingRecordedCosts() { return _showRecordedCosts; }
    const bool IsShowingRecordedDiscovereds() { return _showRecordedDiscovereds; }

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
    bool _showNodeCenter;
    bool _showNodeCoordinates;
    bool _showNodeWeights;

    bool _showRecordedVisiteds;
    bool _showRecordedCosts;
    bool _showRecordedDiscovereds;

	Tile tile;
};

template<typename Grid>
GridPainter<Grid>::GridPainter(const typename Grid::Tile& initialTile, sf::Vector2u _windowSize, unsigned int maxZoomFactor, unsigned int minZoomFactor) : tile(initialTile), windowSize(_windowSize) {
    windowCenter = sf::Vector2i(_windowSize.x / 2, _windowSize.y / 2);

    SetMaxZoom(maxZoomFactor);
    SetMinZoom(minZoomFactor);

    _showVisualGrid = false;
    _showNodeCenter = false;
    _showNodeCoordinates = false;
    _showNodeWeights = true;

    _showRecordedVisiteds = false;
    _showRecordedCosts = false;
    _showRecordedDiscovereds = true;
}


