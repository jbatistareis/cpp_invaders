#include "player.h"

Player::Player()
{
	// replace with a sprite
	player = sf::RectangleShape(sf::Vector2f(24, 10));
	player.setFillColor(sf::Color::Green);
}

void Player::moveLeft(float& deltaTime) {
	if (player.getPosition().x > 5)
		player.move(-speed * deltaTime, 0);
}

void Player::moveRight(float& deltaTime) {
	if (player.getPosition().x < 611)
		player.move(speed * deltaTime, 0);
}

void Player::shoot(float& deltaTime) {
	if (timeSinceLastShot == 0) {
		timeSinceLastShot += deltaTime;

		sf::RectangleShape projectile(sf::Vector2f(6, 10));
		projectile.setPosition(sf::Vector2f((player.getPosition().x + 9), 462));

		projectiles.push_back(projectile);
	}
}

void Player::resetPositon()
{
	player.setPosition(sf::Vector2f(308, 465));
}

int Player::getLives() {
	return lives;
}

void Player::reduceLive()
{
	if (!hit) {
		hit = true;
		lives--;
		// play animation
		if (lives >= 0)
			resetPositon();

		hit = false;
	}
}

void Player::oneUp() {
	lives++;
	// play animation
}

std::vector<sf::RectangleShape>& Player::getProjectiles()
{
	return projectiles;
}

void Player::outsideProjCountAdd()
{
	outsideProjCount++;
}

void Player::outsideProjCountReset()
{
	outsideProjCount = 0;
}

int Player::getOutsideProjCount() {
	return outsideProjCount;
}

double Player::getProjSpeed()
{
	return projSpeed;
}

sf::FloatRect Player::getBounds()
{
	return player.getGlobalBounds();
}



void Player::draw(sf::RenderWindow& window, float& deltaTime)
{
	if (timeSinceLastShot > 0) {
		timeSinceLastShot += deltaTime;
		if (timeSinceLastShot > shotInvterval)
			timeSinceLastShot = 0;
	}

	window.draw(player);
}