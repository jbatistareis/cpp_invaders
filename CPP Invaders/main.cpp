#include <iostream>
#include <SFML/Graphics.hpp>
#include "enemy.h"

sf::Clock deltaClock;
sf::Time deltaTime;

// player settings
sf::RectangleShape player(sf::Vector2f(24, 10));
std::vector<sf::RectangleShape> playerProjectiles;
bool enemyHit = false;
double timeSinceLastShot = 0;
int playerLives = 3;
double playerSpeed = 250;
double playerProjSpeed = 5;
int playerOutsideProjCount = 0;

// enemy settings
Enemy enemies[5][12] = {};
std::vector<sf::RectangleShape> enemyProjectiles;
bool playerHit = false;
double enemiesCurrentXSpeed;
double enemiesCurrentYSpeed;
double enemiesXSpeed = 0.5;
double enemiesYSpeedFactor = 15;
double enemiesIncrSpeed = 0.07;
double enemiesProjSpeed = 6;
int enemyOutsideProjCount = 0;

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
			Enemy enemy;
			enemy.alive = true;
			enemy.hit = false;
			enemy.shape = sf::CircleShape(15);
			enemy.shape.setFillColor(sf::Color::Green);
			enemy.shape.setPosition(sf::Vector2f(90 + (40 * y), 80 + (40 * x)));

			enemies[x][y] = enemy;
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
			if (timeSinceLastShot == 0) {
				timeSinceLastShot += deltaTime.asSeconds();

				sf::RectangleShape projectile(sf::Vector2f(6, 10));
				projectile.setPosition(sf::Vector2f((player.getPosition().x + 9), 462));

				playerProjectiles.push_back(projectile);
			}

		if (timeSinceLastShot > 0) {
			timeSinceLastShot += deltaTime.asSeconds();
			if (timeSinceLastShot > 0.5)
				timeSinceLastShot = 0;
		}

		// boundaries
		for (int x = 4; x >= 0; x--) {
			// min
			for (int y = 0; y < 12; y++)
				if (enemies[x][y].alive) {
					// x
					if (enemies[x][y].shape.getPosition().x < 5) {
						enemiesHitXBound = false;
						break;
					}
					// y
					if (enemies[x][y].shape.getPosition().y < 40) {
						enemiesHitYBound = false;
						break;
					}
				}

			// max
			for (int j = 11; j >= 0; j--)
				if (enemies[x][j].alive) {
					// x
					if (enemies[x][j].shape.getPosition().x > 605) {
						enemiesHitXBound = true;
						break;
					}
					// y
					if (enemies[x][j].shape.getPosition().y > 300) {
						enemiesHitYBound = true;
						break;
					}
				}
		}

		// move enemies
		enemiesCurrentXSpeed = enemiesHitXBound ? -enemiesXSpeed : enemiesXSpeed;
		enemiesCurrentYSpeed = enemiesHitYBound ? -enemiesXSpeed / enemiesYSpeedFactor : enemiesXSpeed / enemiesYSpeedFactor;
		for (int x = 0; x < 5; x++)
			for (int y = 0; y < 12; y++)
				enemies[x][y].shape.move(sf::Vector2f(enemiesCurrentXSpeed, enemiesCurrentYSpeed));

		// shooting enemies

		// draw start
		window.clear();

		window.draw(player);

		// enemies
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 12; j++)
				if (enemies[i][j].alive)
					window.draw(enemies[i][j].shape);

		// enemy projectiles
		for (auto iterator = enemyProjectiles.begin(); iterator != enemyProjectiles.end(); iterator++)
			if ((*iterator).getPosition().y < 500) {
				(*iterator).move(sf::Vector2f(0, enemiesProjSpeed));
				window.draw((*iterator));

				if ((*iterator).getPosition().y > 500)
					enemyOutsideProjCount++;

				// collision
				if (player.getGlobalBounds().intersects((*iterator).getGlobalBounds())) {
					playerHit = true;
					playerLives--;
				}
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
						if (enemies[x][y].alive && enemies[x][y].shape.getGlobalBounds().intersects((*iterator).getGlobalBounds())) {
							enemies[x][y].alive = false;
							enemiesXSpeed += enemiesIncrSpeed;

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