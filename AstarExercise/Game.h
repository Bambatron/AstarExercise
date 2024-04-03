#pragma once
#include <iostream>


enum class GameState {
    Normal,
    Searching,
};

class Game {
    Game();

    template<typename Graph> void SaveCurrentGraph(const Graph& graph) const {}
    //std::string& LoadNewGrid() {return std::string(); }

private:
    GameState cState;

};

