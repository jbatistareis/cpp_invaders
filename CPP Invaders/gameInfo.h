#pragma once
class GameInfo
{
public:
	enum GameState { TITLE, PLAYING, GAMEOVER };
	GameState currentState = PLAYING;
};

