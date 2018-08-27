#pragma once
#include <SFML/Graphics.hpp>

class Enemy : public sf::Drawable, public sf::Transformable
{
public:
	Enemy();
	Enemy(double x, double y, int _lives);
	void takeHit();
	bool isAlive();

	const sf::Vector2f& getPosition() const;
	void move(const sf::Vector2f& offset);
	sf::FloatRect getGlobalBounds() const;

private:
	sf::CircleShape shape = sf::CircleShape(15);
	bool alive = true;
	int lives;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
