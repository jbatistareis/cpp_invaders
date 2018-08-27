#pragma once
#include <SFML/Graphics.hpp>
#include "gameInfo.h"
#include "player.h"
#include "enemy.h"

class GameScreen
{
public:
	GameScreen(GameInfo &_gameinfo);
	void start();
	void movePlayerLeft(float deltaTime);
	void movePlayerRight(float deltaTime);
	void playerShoot(float deltaTime);
	void draw(sf::RenderWindow& window, float deltaTime);

private:
	GameInfo & gameInfo;

	bool waveOver = false;
	bool gameOver = false;
	int wave = 1;
	int score = 0;

	int deadEnemies = 0;

	Player player;
	Enemy enemies[5][12] = {};

	// enemy settings
	std::vector<sf::RectangleShape> enemyProjectiles;
	double enemiesCurrentXSpeed;
	double enemiesCurrentYSpeed;
	double enemiesXSpeed = 0.5;
	double enemiesYSpeedFactor = 15;
	double enemiesIncrSpeed = 0.07;
	double enemiesProjSpeed = 6;
	double enemiesProjIncrSpeed = 0.07;
	double enemiesShotFreq = 1.0;
	double enemiesShotIncrFreq = 0.01;
	double enemiesTimeSinceLastShot = 0;
	int enemyAimArea = 20;
	int enemyOutsideProjCount = 0;
	int shootingEnemyId;
	Enemy shootingEnemy;

	// move boundaries
	bool enemiesHitXBound = false;
	bool enemiesHitYBound = false;

};

