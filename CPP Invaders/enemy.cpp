#include "enemy.h"

Enemy::Enemy()
{
}

Enemy::Enemy(double x, double y, int _lives)
{
	// replace with a sprite
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(sf::Vector2f(90 + (40 * y), 80 + (40 * x)));

	lives = _lives;
}

void Enemy::takeHit()
{
	// play animation
	if (--lives <= 0)
		alive = false;
}

bool Enemy::isAlive()
{
	return alive;
}

const sf::Vector2f & Enemy::getPosition() const
{
	return shape.getPosition();
}

void Enemy::move(const sf::Vector2f & offset)
{
	shape.move(offset);
}

sf::FloatRect Enemy::getGlobalBounds() const
{
	return shape.getGlobalBounds();
}

void Enemy::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(shape);
}
