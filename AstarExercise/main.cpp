#pragma once

#include <typeindex>

#include "SaveOpenUtilities.h"
#include "Pathfinder.h"
#include "HexGrid.h"
#include "HexPainter.h"
#include "LabelMenu.h"

enum GameState {
    Normal,
    Searching,
};

int main() {
    std::cout << "Hello world" << std::endl;
    
   GameState gameState = GameState::Normal;

   sf::ContextSettings settings;
   settings.antialiasingLevel = 8;
   sf::RenderWindow window(sf::VideoMode(1024, 768), "Hexagon Example", sf::Style::Default, settings);

   HexGrid grid("BasicMapWeighted.json");
   bool _selectedHex = false;
   Hex selectedHex(0, 0);

   HexPainter painter(30, window.getSize());
   float movementSpeed = 10;
   float scrollSpeed = 10;

   Pathfinder<HexGrid, Hex> pathFinder(new AstarStrategy<HexGrid, Hex>{});
   bool _startSelected;
   Hex start(3,1);
   bool _goalSelected;
   Hex goal(3,-1);
   pathFinder.SetStart(start);
   pathFinder.SetGoal(goal);
   while (!pathFinder.MakeStep(grid)) {}
   
   SearchRecord<Hex> record = pathFinder.GetCurrentRecord();
   std::cout << record.completed << " " << record.currentNode.PrintOut() << std::endl;
   pathFinder.MoveAheadRecord();
   record = pathFinder.GetCurrentRecord();
   std::cout << record.completed << " " << record.currentNode.PrintOut() << std::endl;
   pathFinder.MoveAheadRecord();
   record = pathFinder.GetCurrentRecord();
   std::cout << record.completed << " " << record.currentNode.PrintOut() << std::endl;
   pathFinder.MoveBackwardRecord();
   record = pathFinder.GetCurrentRecord();
   std::cout << record.completed << " " << record.currentNode.PrintOut() << std::endl;


   LabelMenu label(sf::Vector2f(10, 10), sf::Vector2f(10, 10));
   Hex labeledHex;
    
    //std::type_index objType = typeid(pathFinder);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }

            if (e.type == sf::Event::KeyPressed) {  //ProcessInputKeyboard(e.key.code);
                if (e.key.code == sf::Keyboard::Left) {
                    painter.MoveCamera(+movementSpeed, 0.0f);
                }
                if (e.key.code == sf::Keyboard::Up) {
                    painter.MoveCamera(0.0f, +movementSpeed);
                }
                if (e.key.code == sf::Keyboard::Down) {
                    painter.MoveCamera(0.0f, -movementSpeed);
                }
                if (e.key.code == sf::Keyboard::Right) {
                    painter.MoveCamera(-movementSpeed, 0.0f);
                }

                if (e.key.code == sf::Keyboard::R) {
                    SaveGrid(grid);
                }
                if (e.key.code == sf::Keyboard::T) {
                    std::string filename = OpenGrid();
                    if (!filename.empty()) {
                        grid = HexGrid(filename);
                    }
                }

                if (e.key.code == sf::Keyboard::F) {
                    painter.ToggleVisualGrid();
                }
                if (e.key.code == sf::Keyboard::G) {
                    painter.ToggleHexCenter();
                }
                if (e.key.code == sf::Keyboard::H) {
                    painter.ToggleHexCoordinates();
                }

                if (gameState == GameState::Normal) {
                    if (e.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }

                    if (e.key.code == sf::Keyboard::Add && _selectedHex) {
                        grid.Increase(selectedHex);
                    }
                    if (e.key.code == sf::Keyboard::Subtract && _selectedHex) {
                        grid.Decrease(selectedHex);
                    }

                    if (e.key.code == sf::Keyboard::Q) {    //Start Astar search
                        pathFinder.SwitchFuntion(new AstarStrategy<HexGrid, Hex>);
                        gameState = GameState::Searching;
                    }
                    if (e.key.code == sf::Keyboard::W) {    //Start Dijkstra search
                        gameState = GameState::Searching;
                    }
                }
                
                else if (gameState == GameState::Searching) {
                    if (e.key.code == sf::Keyboard::Escape) {
                        pathFinder.Reset();
                        gameState = GameState::Normal;
                    }
                    
                    if (e.key.code == sf::Keyboard::A) {
                        //Move backward a step in the search
                    }
                    if (e.key.code == sf::Keyboard::D) {
                        //move ahead a step in the search
                    }
                    if (e.key.code == sf::Keyboard::P) {
                        //Switch between Manual or Automatical Search progression
                    }

                    if (e.key.code == sf::Keyboard::I) {
                        pathFinder.ClearStart();
                    }
                    if (e.key.code == sf::Keyboard::O) {
                        pathFinder.ClearGoal();
                    }
                    if (e.key.code == sf::Keyboard::R) {
                        pathFinder.Reset();
                    }
                }
            }

            if (e.type == sf::Event::MouseButtonPressed) {  //ProcessInputMouse(e.mouseButton.button)
                if (e.mouseButton.button == sf::Mouse::Left) {
                    if (gameState == GameState::Normal) {
                        sf::Vector2i pixelPos = sf::Mouse::getPosition(window); //Get the current mouse position in the window
                        std::cout << "Pixel pos: " << pixelPos.x << ", " << pixelPos.y << std::endl;
                        Hex tmp(PixelToHex(sf::Vector2f(pixelPos), painter.GetTile().Radius(), painter.GetWindowCenter()));
                        if (grid.IsInBounds(tmp) && tmp != selectedHex) {
                            std::cout << "Selected hex is: " << tmp.PrintOut() << std::endl;
                            _selectedHex = true;
                            selectedHex = tmp;
                        }
                        else {
                            std::cout << "Deselect" << std::endl;
                            _selectedHex = false;
                        }
                    }
                    else if (gameState == GameState::Searching) {
                        sf::Vector2i pixelPos = sf::Mouse::getPosition(window); //Get the current mouse position in the window
                        Hex tmp(PixelToHex(sf::Vector2f(pixelPos), painter.GetTile().Radius(), painter.GetWindowCenter()));

                        if (!pathFinder.IsStartSelected()) {
                            pathFinder.SetStart(tmp);
                        }
                        else if(!pathFinder.IsGoalSelected()){
                            pathFinder.SetGoal(tmp);
                        }
                    }
                }

                if (e.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window); //Get the current mouse position in the window
                    std::cout << "Pixel pos: " << pixelPos.x << ", " << pixelPos.y << std::endl;
                    Hex tmp(PixelToHex(sf::Vector2f(pixelPos), painter.GetTile().Radius(), painter.GetWindowCenter()));
                    if (grid.IsInBounds(tmp)) {
                        if (label.IsOpen()) {
                            if (tmp == labeledHex) {
                                label.Close();
                            }
                            else {
                                label.SetPosition(sf::Vector2f(pixelPos));
                                label.ClearParameters();
                                label.AddParameters("Coordinates: ", tmp.q, tmp.r, tmp.s);
                                label.AddParameters("Weight: ", grid.Weight(tmp));
                                labeledHex = tmp;
                            }
                        }
                        else {
                            if (tmp != labeledHex) {
                                label.SetPosition(sf::Vector2f(pixelPos));
                                label.ClearParameters();
                                label.AddParameters("Coordinates: ", tmp.q, tmp.r, tmp.s);
                                label.AddParameters("Weight: ", grid.Weight(tmp));
                                labeledHex = tmp;
                            }
                            label.Open();
                        }

                    }
                    else {
                        label.Close();
                    }
                }
            }

            if (e.type == sf::Event::MouseWheelScrolled) {  //ProcessInputWheel(e.mouseWheel.delta)M
                if (e.mouseWheel.x > 0) {
                    painter.Zoom(scrollSpeed);
                }
                else if (e.mouseWheel.x < 0) {
                    painter.Zoom(-scrollSpeed);
                }
            }
        }

        //Render
        window.clear();

        painter.Render(grid, window);
        if (label.IsOpen()) {
            label.Render(window);
        }

        window.display();
    }

    return 0;
}

//PATHFINDER EXAMPLE
/*
    Pathfinder<HexGrid, Hex> pathFinder(new AstarStrategy<HexGrid, Hex>{});
    pathFinder.SetStart(start);
    while (!pathFinder.MakeStep(grid, goal)) {
        //Stuff
    }
    std::cout << "Setted start: " << pathFinder.GetStart().PrintOut() << std::endl;
    auto pathTaken = pathFinder.PathTaken(goal);
    std::cout << "Came form: " << "\t";
    for (auto it : pathTaken) {
        std::cout << it.PrintOut() << "\t";
    }
    std::cout << "Cost so far: " << pathFinder.GetCostAtLocation(goal) << std::endl;
*/

//LABELMENU EXAMPLE
/*
    labelMenu.DrawParameters("Goal coordinates", goal.q, goal.r, goal.s);
    // Print the contents of the info vector
    for (const auto& pair : labelMenu.GetInfo()) {
        std::cout << "Name: " << pair.first << ", Parameters: ";
        for (const auto& str : pair.second) {
            std::cout << str << " ";
        }
        std::cout << std::endl;
    }
*/

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
        if (event.key.code == sf::Keyboard::G)
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

    if (event.key.code == sf::Keyboard::Escape)
        window.close();
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