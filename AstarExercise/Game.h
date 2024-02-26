#pragma once

enum GameState {
	Normal,
	Searching,
};

class Game {
public:
	Game();
	~Game();



private:
	GameState cState;
};

Game::Game() {
	//RunTest

}

Game::~Game() {
}