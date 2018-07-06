#pragma once

class Game {
public:
	enum GameState { TITLE, PLAYING, GAMEOVER };
	GameState currentState = PLAYING;
};