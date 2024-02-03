#pragma once

#include "SaveOpenUtilities.h"
#include "Pathfinder.h"
#include "HexGrid.h"
#include "Painter.h"

int main() {

    HexGrid grid("BasicMapWeighted.json");
    Hex start(1, 1);
    Hex goal(-1, 0);


    /*PATHFINDER EXAMPLE
    * Pathfinder<HexGrid, Hex> pathFinder(new AstarStrategy<HexGrid, Hex>{});
    * pathFinder.SetStart(start);
    * while (!pathFinder.MakeStep(grid, goal)) { 
    *   //Stuff
    * }
    * std::cout << "Setted start: " << pathFinder.GetStart().PrintOut() << std::endl;
    * auto pathTaken = pathFinder.PathTaken(goal);
    * std::cout << "Came form: " << "\t";
    * for (auto it : pathTaken) {
    *   std::cout << it.PrintOut() << "\t";
    * }
    * std::cout << "Cost so far: " << pathFinder.GetCostAtLocation(goal) << std::endl;
    */

    return 0;
}
/*
    std::cout << "Hello world" << std::endl;
   
   //Creating window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Hexagon Example", sf::Style::Default, settings);

    //Creating grid
    WeightedHexGrid* grid = new WeightedHexGrid();
    
    Map<WeightedHexGrid> map(window.getSize());
    
    bool _selectedHex = false;
    Hex selectedHex(0, 0);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
            if (e.type == sf::Event::KeyPressed) {
                if(e.key.code == sf::Keyboard::Escape) {
                    window.close();
                }

                if (e.key.code == sf::Keyboard::R) {
                    SaveGrid(*grid);
                }
                if (e.key.code == sf::Keyboard::T) {
                    std::string filename = OpenGrid();
                    if (!filename.empty()) {
                        grid = new WeightedHexGrid(filename);
                    }
                }

                if (e.key.code == sf::Keyboard::Left) {
                    //map.MoveCamera(-5.0f, 0.0f);
                }
                if (e.key.code == sf::Keyboard::Up) {
                    //map.MoveCamera(0.0f, -5.0f);
                }
                if (e.key.code == sf::Keyboard::Down) {
                    //map.MoveCamera(0.0f, +5.0f);
                }
                if (e.key.code == sf::Keyboard::Right) {
                   //map.MoveCamera(5.0f, 0.0f);
                }
                if (e.key.code == sf::Keyboard::F) {
                    map.ToggleVisualGrid();
                }
                if (e.key.code == sf::Keyboard::G) {
                    map.ToggleHexCenter();
                }
                if (e.key.code == sf::Keyboard::H) {
                    map.ToggleHexCoordinates();
                }

                if (_selectedHex) {
                    if (e.key.code == sf::Keyboard::Add) {
                        std::cout << "Increase hex: " << selectedHex.Read() << std::endl;
                        grid->Increase(selectedHex);
                    }
                    if (e.key.code == sf::Keyboard::Subtract) {
                        std::cout << "Decrease hex: " << selectedHex.Read() << std::endl;
                        grid->Decrease(selectedHex);
                    }
                }
            }
        
            if (e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window); //Get the current mouse position in the window
                    std::cout << "Pixel pos: " << pixelPos.x << ", " << pixelPos.y << std::endl;
                    /*sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);  //Convert it to world coordinates
                    std::cout << "World pos: " << worldPos.x << ", " << worldPos.y << std::endl;
                    Hex tmp(map.PixelToHex(worldPos, window));
                    Hex tmp(map.PixelToHex(sf::Vector2f(pixelPos), window));
                    std::cout << tmp.Read() << std::endl;
                    if (grid->IsInBounds(tmp) && tmp != selectedHex) {
                        std::cout << "Selected hex is: " << tmp.Read() << std::endl;
                        _selectedHex = true;
                        selectedHex = tmp;
                    }
                    else {
                        std::cout << "Deselect" << std::endl;
                        _selectedHex = false;
                    }
                }
            }
            if (e.type == sf::Event::MouseWheelScrolled) {
                if (e.mouseWheel.x > 0) {
                    //map.Zoom(0.85);
                    map.Zoom(5);
                }
                else if (e.mouseWheel.x < 0) {
                    //map.Zoom(1.15);
                    map.Zoom(-5);
                }
            }
        }

        //Render
        window.clear();

        map.Render(*grid, window);
        
        window.display();
    }

    return 0;
}

	/*   

    //Search stuff
    bool _breadthFirstGoal = false;
    bool _astarSearch = false;
    Hex start;
    bool _selectingStartingHex = false;
    bool _renderStartHex = false;
    Hex goal;
    bool _selectingGoalHex = false;
    bool _renderGoalHex = false;
    std::queue<Hex> frontier;
    PriorityQueue<Hex, int> priorityFrontier;
    std::set<Hex> reached;
    std::unordered_map<Hex, Hex> cameFrom;
    std::vector<Hex> reconstructedPath;
    std::unordered_map<Hex, int> costSoFar;
    
    bool _startSearch = false;
    bool _showPath = true;

    int rad = map.GetTile().Radius() / 5;
    sf::CircleShape circle(rad);
    circle.setOrigin(rad, rad);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(1);

    sf::Clock gClcok;
    sf::Time elapsedTime = sf::Time::Zero;
    sf::Time frameRate = sf::seconds(1.f/1.f);

    // Main loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {  //Check which mouse button is pressed
                if (_selectingStartingHex) {
                    sf::Vector2i mousePosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
                    
                    start = map.PixelToHex(mousePosition);
                    frontier.push(start);
                    reached.insert(start);
                    priorityFrontier.put(start, 0);
                    cameFrom[start] = start;
                    costSoFar[start] = 0;

                    std::cout << "Selected start: " << start.Read() << std::endl;
                    _selectingStartingHex = false;
                    _renderStartHex = true;
                    if (_breadthFirstGoal || _astarSearch) {
                        _selectingGoalHex = true;
                        std::cout << "Select a goal hex" << std::endl;
                    }
                    else {
                        _startSearch = true;
                    }
                }
                else if (_selectingGoalHex) {
                    sf::Vector2i mousePosition = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
                    goal = map.PixelToHex(mousePosition);
                    std::cout << "Selected goal: " << goal.Read() << std::endl;
                    
                    _selectingGoalHex = false;
                    _renderGoalHex = true;
                    _startSearch = true;
                }
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    sf::Vector2i mousePos = sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
                    std::cout << "Mouse wheel scrolled on Hex: "<< map.PixelToHex(mousePos).Read() << std::endl;
                    if (event.mouseWheelScroll.delta >0) {
                        std::cout << "Increase" << std::endl;
                    }
                    else {
                        std::cout << "Decrease" << std::endl;
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::G) {
                    _showVisualGrid = !(_showVisualGrid);
                }

                if (event.key.code == sf::Keyboard::P) { //starts a search
                    std::cout << "Breadth First Search" << std::endl;
                    std::cout << "Select a staring hex" << std::endl;
                    _selectingStartingHex = true;
                }
                if (event.key.code == sf::Keyboard::L) { //starts a search
                    std::cout << "Breadth First Search to Goal" << std::endl;
                    std::cout << "Select a staring hex" << std::endl;
                    _selectingStartingHex = true;
                    _breadthFirstGoal = true;
                }
                if (event.key.code == sf::Keyboard::O) {
                    std::cout << "Astar Search" << std::endl;
                    std::cout << "Select a staring hex" << std::endl;
                    _selectingStartingHex = true;
                    _astarSearch = true;
                }

                if (event.key.code == sf::Keyboard::M) {
                    _startSearch = false;
                    _breadthFirstGoal = false;
                    _renderStartHex = false;
                    _renderGoalHex = false;
                    _showPath = false;
                }

                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        elapsedTime += gClcok.restart();

        if (_startSearch && _breadthFirstGoal) {
            if (!RecursiveBFSToGoalVisual(grid, goal, frontier, cameFrom)) {
                reconstructedPath = ReconstructPath(cameFrom, start, goal);
                _startSearch = false;
                _breadthFirstGoal = false;
                _showPath = true;
            }
        }
        if (_startSearch && _astarSearch) {
            if (RecursiveAstarSearchVisual(grid, goal, priorityFrontier, cameFrom, costSoFar)) {
                reconstructedPath = ReconstructPath(cameFrom, start, goal);
                _startSearch = false;
                _astarSearch = false;
                _showPath = true;
            }
        }
        while (elapsedTime >= frameRate && _startSearch && !_breadthFirstGoal && !_astarSearch) {
            if (_startSearch) {
                RecursiveBFSVisual(grid, frontier, reached);
            }

            elapsedTime -= frameRate;
        }

        // Clear the window
        window.clear();

        // Draw the hexagon
        map.Render(grid, window);

        if (_showVisualGrid) {
            map.RenderVisualGrid(window);
        }
        if (_startSearch && !_breadthFirstGoal && !_astarSearch) {
            for (auto it : reached) { //Render closed nodes
                circle.setPosition(map.HexToPixel(it));
                circle.setFillColor(sf::Color::Red);
                window.draw(circle);
            }

            // Create a temporary copy of the queue
            std::queue<Hex> tempFrontier = frontier;
            // Visit elements in the temporary copy
            while (!tempFrontier.empty()) { //Render discovered and open nodes
                circle.setPosition(map.HexToPixel(tempFrontier.front()));
                circle.setFillColor(sf::Color::Blue);
                window.draw(circle);
                tempFrontier.pop();
            }
        }
        if (_showPath) {
            for (auto it : reconstructedPath) {
                circle.setPosition(map.HexToPixel(it));
                circle.setFillColor(sf::Color(244, 185, 62));
                window.draw(circle);
            }
        }
        if (_renderStartHex) {
            text.setString("S");
            text.setPosition(map.HexToPixel(start));
            window.draw(text);
        }
        if (_renderGoalHex) {
            text.setString("G");
            text.setPosition(map.HexToPixel(goal));
            window.draw(text);
        }

        // Display the contents of the window
        window.display();
    }

	return 0;
}*/