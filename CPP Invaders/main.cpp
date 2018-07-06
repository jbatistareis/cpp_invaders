#pragma once
#include <iostream>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include "game.h"
#include "gameScreen.h"
#include "player.h"
#include "enemy.h"

Game game;
sf::Clock deltaClock;
sf::Time deltaTime;

bool waveOver = false;
bool gameOver = false;
int wave = 1;
int score = 0;

int deadEnemies = 0;

// player settings
sf::RectangleShape player(sf::Vector2f(24, 10));
std::vector<sf::RectangleShape> playerProjectiles;
int playerLives = 3;
double playerTimeSinceLastShot = 0;
double playerSpeed = 250;
double playerProjSpeed = 4;
double shotInvterval = 0.5;
int playerOutsideProjCount = 0;

// enemy settings
Enemy enemies[5][12] = {};
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

// misc
bool playerHit = false;
bool enemyHit = false;

// move boundaries
bool enemiesHitXBound = false;
bool enemiesHitYBound = false;

int main() {
	sf::RenderWindow window(sf::VideoMode(640, 480), "CPP Invaders");
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);

	player.setPosition(sf::Vector2f(308, 465));
	player.setFillColor(sf::Color::Green);

	// instantiate enemies
	for (int x = 0; x < 5; x++)
		for (int y = 0; y < 12; y++) {
			enemies[x][y] = Enemy(x, y);
		}

	while (window.isOpen()) {
		deltaTime = deltaClock.restart();

		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		// controls
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			if (player.getPosition().x < 611)
				player.move(playerSpeed * deltaTime.asSeconds(), 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			if (player.getPosition().x > 5)
				player.move(-playerSpeed * deltaTime.asSeconds(), 0);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			if (playerTimeSinceLastShot == 0) {
				playerTimeSinceLastShot += deltaTime.asSeconds();

				sf::RectangleShape projectile(sf::Vector2f(6, 10));
				projectile.setPosition(sf::Vector2f((player.getPosition().x + 9), 462));

				playerProjectiles.push_back(projectile);
			}

		if (playerTimeSinceLastShot > 0) {
			playerTimeSinceLastShot += deltaTime.asSeconds();
			if (playerTimeSinceLastShot > shotInvterval)
				playerTimeSinceLastShot = 0;
		}

		// boundaries and dead count
		for (int x = 4; x >= 0; x--) {
			// min
			for (int y = 0; y < 12; y++)
				if (enemies[x][y].alive) {
					// x
					if (enemies[x][y].getPosition().x < 5) {
						enemiesHitXBound = false;
						break;
					}
					// y
					if (enemies[x][y].getPosition().y < 40) {
						enemiesHitYBound = false;
						break;
					}
				}
				else {
					deadEnemies++;
				}

				// max
				for (int j = 11; j >= 0; j--)
					if (enemies[x][j].alive) {
						// x
						if (enemies[x][j].getPosition().x > 605) {
							enemiesHitXBound = true;
							break;
						}
						// y
						if (enemies[x][j].getPosition().y > 300) {
							enemiesHitYBound = true;
							break;
						}
					}
		}

		// wave signal
		if (deadEnemies >= 60)
			waveOver = true;

		// move enemies
		enemiesCurrentXSpeed = enemiesHitXBound ? -enemiesXSpeed : enemiesXSpeed;
		enemiesCurrentYSpeed = enemiesHitYBound ? -enemiesXSpeed / enemiesYSpeedFactor : enemiesXSpeed / enemiesYSpeedFactor;
		for (int x = 0; x < 5; x++)
			for (int y = 0; y < 12; y++)
				enemies[x][y].move(sf::Vector2f(enemiesCurrentXSpeed, enemiesCurrentYSpeed));

		// shooting enemies
		// pick a random number from 1-60
		// enemies[num / 12][num % 12]
		if (enemiesTimeSinceLastShot >= enemiesShotFreq) {
			enemiesTimeSinceLastShot = 0;

			do {
				shootingEnemyId = (rand() % 59) + 1;
				shootingEnemy = enemies[shootingEnemyId / 12][shootingEnemyId % 12];
			} while (!shootingEnemy.alive);

			sf::RectangleShape projectile(sf::Vector2f(6, 10));
			projectile.setPosition(sf::Vector2f((
				shootingEnemy.getPosition().x + 12),
				shootingEnemy.getPosition().y + 15));

			enemyProjectiles.push_back(projectile);
		}
		else {
			enemiesTimeSinceLastShot += deltaTime.asSeconds();
		}

		// draw start
		window.clear();

		window.draw(player);

		// enemies
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 12; j++)
				if (enemies[i][j].alive)
					window.draw(enemies[i][j]);

		// enemy projectiles
		for (auto enemyProjIter = enemyProjectiles.begin(); enemyProjIter != enemyProjectiles.end(); enemyProjIter++)
			if ((*enemyProjIter).getPosition().y < 500) {
				(*enemyProjIter).move(sf::Vector2f(0, enemiesProjSpeed));
				window.draw((*enemyProjIter));

				if ((*enemyProjIter).getPosition().y > 500)
					enemyOutsideProjCount++;

				// collision with player projectile
				for (auto playerProjIter = playerProjectiles.begin(); playerProjIter != playerProjectiles.end(); playerProjIter++) {
					if ((*playerProjIter).getGlobalBounds().intersects((*enemyProjIter).getGlobalBounds())) {
						(*playerProjIter).setPosition(sf::Vector2f(0, -1));
						(*enemyProjIter).setPosition(sf::Vector2f(0, 501));
					}
				}

				// collision with player
				if (player.getGlobalBounds().intersects((*enemyProjIter).getGlobalBounds())) {
					playerHit = true;
					playerLives--;
				}

				// game over
				if (playerLives <= 0)
					game.currentState = Game::GAMEOVER;
			}

		if (enemyOutsideProjCount >= enemyProjectiles.size()) {
			enemyOutsideProjCount = 0;
			enemyProjectiles.clear();
			enemyProjectiles.shrink_to_fit();
		}

		//player projectiles		
		for (auto iterator = playerProjectiles.begin(); iterator != playerProjectiles.end(); iterator++)
			if ((*iterator).getPosition().y > 0) {
				(*iterator).move(sf::Vector2f(0, -playerProjSpeed));
				window.draw((*iterator));

				if ((*iterator).getPosition().y < 0)
					playerOutsideProjCount++;

				// collision
				for (int x = 4; x >= 0; x--) {
					for (int y = 0; y < 12; y++) {
						if (enemies[x][y].alive && enemies[x][y].getGlobalBounds().intersects((*iterator).getGlobalBounds())) {
							enemies[x][y].alive = false;
							enemiesXSpeed += enemiesIncrSpeed;
							enemiesShotFreq -= enemiesShotIncrFreq;

							(*iterator).setPosition(sf::Vector2f(-1, -1));

							enemyHit = true;
							break;
						}
					}

					if (enemyHit) {
						enemyHit = false;
						break;
					}
				}
			}

		if (playerOutsideProjCount >= playerProjectiles.size()) {
			playerOutsideProjCount = 0;
			playerProjectiles.clear();
			playerProjectiles.shrink_to_fit();
		}

		window.display();
	}

	return 0;
}