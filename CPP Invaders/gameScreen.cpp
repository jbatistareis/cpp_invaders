#include "gameScreen.h"

GameScreen::GameScreen(GameInfo & _gameinfo) : gameInfo(_gameinfo) { }

void GameScreen::start()
{
	// instantiate enemies
	// wave message with a delay
	for (int x = 0; x < 5; x++)
		for (int y = 0; y < 12; y++) {
			// make patterns
			enemies[x][y] = Enemy(x, y, 2);
		}

	player = Player();
	player.resetPositon();
}

void GameScreen::movePlayerLeft(float deltaTime)
{
	player.moveLeft(deltaTime);
}

void GameScreen::movePlayerRight(float deltaTime)
{
	player.moveRight(deltaTime);
}

void GameScreen::playerShoot(float deltaTime)
{
	player.shoot(deltaTime);
}

void GameScreen::draw(sf::RenderWindow& window, float deltaTime)
{
	// boundaries and dead count
	for (int x = 4; x >= 0; x--) {
		// min
		for (int y = 0; y < 12; y++)
			if (enemies[x][y].isAlive()) {
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

		// max
		for (int j = 11; j >= 0; j--)
			if (enemies[x][j].isAlive()) {
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
	if (deadEnemies >= 60) {
		waveOver = true;
		wave += 1;
	}

	// move enemies
	enemiesCurrentXSpeed = enemiesHitXBound ? -enemiesXSpeed : enemiesXSpeed;
	enemiesCurrentYSpeed = enemiesHitYBound ? -enemiesXSpeed / enemiesYSpeedFactor : enemiesXSpeed / enemiesYSpeedFactor;
	for (int x = 0; x < 5; x++)
		for (int y = 0; y < 12; y++)
			enemies[x][y].move(sf::Vector2f(enemiesCurrentXSpeed, enemiesCurrentYSpeed));

	// shooting enemies
	// pick a random number from 1-60
	// enemies[num / 12][num % 12]
	if (!waveOver) {
		if (enemiesTimeSinceLastShot >= enemiesShotFreq) {
			enemiesTimeSinceLastShot = 0;

			do {
				shootingEnemyId = (rand() % 59) + 1;
				shootingEnemy = enemies[shootingEnemyId / 12][shootingEnemyId % 12];
			} while (!shootingEnemy.isAlive());

			sf::RectangleShape projectile(sf::Vector2f(6, 10));
			projectile.setPosition(sf::Vector2f((
				shootingEnemy.getPosition().x + 12),
				shootingEnemy.getPosition().y + 15));

			enemyProjectiles.push_back(projectile);
		}
		else {
			enemiesTimeSinceLastShot += deltaTime;
		}
	}

	// draw player
	player.draw(window, deltaTime);

	// draw enemies
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 12; j++)
			if (enemies[i][j].isAlive())
				window.draw(enemies[i][j]);

	// enemy projectiles
	for (auto enemyProjIter = enemyProjectiles.begin(); enemyProjIter != enemyProjectiles.end(); enemyProjIter++)
		if ((*enemyProjIter).getPosition().y < 500) {
			(*enemyProjIter).move(sf::Vector2f(0, enemiesProjSpeed));
			window.draw(*enemyProjIter);

			if ((*enemyProjIter).getPosition().y > 500)
				enemyOutsideProjCount++;

			// collision with player projectile
			for (auto playerProjIter = player.getProjectiles().begin(); playerProjIter != player.getProjectiles().end(); playerProjIter++) {
				if ((*playerProjIter).getGlobalBounds().intersects((*enemyProjIter).getGlobalBounds())) {
					(*playerProjIter).setPosition(sf::Vector2f(0, -1));
					(*enemyProjIter).setPosition(sf::Vector2f(0, 501));
				}
			}

			// collision with player
			if (player.getBounds().intersects((*enemyProjIter).getGlobalBounds())) {
				player.reduceLive();

				// game over
				if (player.getLives() < 0)
					gameInfo.currentState = gameInfo.GAMEOVER;
			}
		}

	if (enemyOutsideProjCount >= enemyProjectiles.size()) {
		enemyOutsideProjCount = 0;
		enemyProjectiles.clear();
		enemyProjectiles.shrink_to_fit();
	}

	//player projectiles		
	for (auto iterator = player.getProjectiles().begin(); iterator != player.getProjectiles().end(); iterator++)
		if ((*iterator).getPosition().y > 0) {
			(*iterator).move(sf::Vector2f(0, -player.getProjSpeed()));
			window.draw((*iterator));

			if ((*iterator).getPosition().y < 0)
				player.outsideProjCountAdd();

			// collision
			for (int x = 4; x >= 0; x--) {
				for (int y = 0; y < 12; y++) {
					if (enemies[x][y].isAlive() && enemies[x][y].getGlobalBounds().intersects((*iterator).getGlobalBounds())) {
						(*iterator).setPosition(sf::Vector2f(-1, -1));

						enemies[x][y].takeHit();
						if (!enemies[x][y].isAlive()) {
							enemiesXSpeed += enemiesIncrSpeed;
							enemiesShotFreq -= enemiesShotIncrFreq;
							deadEnemies++;
						}

						break;
					}
				}
			}
		}

	if (player.getOutsideProjCount() >= player.getProjectiles().size()) {
		player.outsideProjCountReset();
		player.getProjectiles().clear();
		player.getProjectiles().shrink_to_fit();
	}
}