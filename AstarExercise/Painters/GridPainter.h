#pragma once

#include <SFML/Graphics.hpp>

#include "../Pathfinder/Pathfinder.h"

enum PainterFlags {
    Visual_Grid,
    Node_Center,
    Node_Coordinates,
    Node_Weights,
    Visited,
    Discovered,
    Costs,
    Path_Taken
};

template<typename Grid>
class GridPainter {
public:
    using tile_t = typename Grid::tile;

    GridPainter(const tile_t& initialTile, sf::Vector2u _windowSize = sf::Vector2u(1024, 768), unsigned int maxZoomFactor = 4, unsigned int minZoomFactor = 40);
    virtual ~GridPainter() {}

	virtual void Render(const Grid& grid, sf::RenderWindow& target) = 0;
    virtual void RenderSearchRecord(const Grid& grid, const SearchRecord<Grid>& record, sf::RenderWindow& target) = 0;

    // Getters
    bool flag(PainterFlags f)const { return flags.at(f); }

    bool IsShowingVisualGrid() const { return flag(PainterFlags::Visual_Grid); }
    bool IsShowingNodeCenter() const { return flag(PainterFlags::Node_Center); }
    bool IsShowingNodeCoordinates() const { return flag(PainterFlags::Node_Coordinates); }
    bool IsShowingNodeWeights() const { return flag(PainterFlags::Node_Weights); }

    bool IsShowingRecordedVisiteds() const { return flag(PainterFlags::Visited); }
    bool IsShowingRecordedCosts() const { return flag(PainterFlags::Costs); }
    bool IsShowingRecordedDiscovereds() const { return flag(PainterFlags::Discovered); }
    bool IsShowingPathTakens() const { return flag(PainterFlags::Path_Taken); }

    const sf::Vector2i GetWindowCenter() { return windowCenter; }

    tile_t& GetTile() { return tile; }

    // Setters
    void toggleFlag(PainterFlags f) { flags.at(f) = !(flags.at(f)); }

    void MoveCamera(sf::Vector2i offset) { windowCenter += offset; }
    void MoveCamera(int offsetX, int offsetY) { MoveCamera(sf::Vector2i(offsetX, offsetY)); }
    virtual void Zoom(float factor) = 0;
    void SetMaxZoom(unsigned int maxZoomFactor) { maxZoom = std::min(windowSize.x, windowSize.y) / maxZoomFactor; }
    void SetMinZoom(unsigned int minZoomFactor) { minZoom = std::min(windowSize.x, windowSize.y) / minZoomFactor; }

protected:
    sf::Vector2u windowSize;
    sf::Vector2i windowCenter;

    unsigned int maxZoom, minZoom;

    std::map<PainterFlags, bool> flags;

    tile_t tile;
};

template<typename Grid>
GridPainter<Grid>::GridPainter(const typename Grid::tile& initialTile, sf::Vector2u _windowSize, unsigned int maxZoomFactor, unsigned int minZoomFactor) : tile(initialTile), windowSize(_windowSize) {
    windowCenter = sf::Vector2i(_windowSize.x / 2, _windowSize.y / 2);

    SetMaxZoom(maxZoomFactor);
    SetMinZoom(minZoomFactor);

    flags[Visual_Grid] = false;
    flags[Node_Center] = false;
    flags[Node_Coordinates] = false;
    flags[Node_Weights] = true;
    flags[Visited] = true;
    flags[Discovered] = true;
    flags[Costs] = true;
    flags[Path_Taken] = false;
}


