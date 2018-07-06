#include "enemy.h"

Enemy::Enemy() {}

Enemy::Enemy(double x, double y)
{
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(sf::Vector2f(90 + (40 * y), 80 + (40 * x)));
}