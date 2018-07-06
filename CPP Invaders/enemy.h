#pragma once
#include <SFML/Graphics.hpp>

class Enemy : public sf::Drawable, public sf::Transformable
{
public:
	bool alive = true;
	bool hit = false;

	Enemy();
	Enemy(double x, double y);

	const sf::Vector2f& getPosition() const;
	void move(const sf::Vector2f& offset);
	sf::FloatRect getGlobalBounds() const;

private:
	sf::CircleShape shape = sf::CircleShape(15);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
