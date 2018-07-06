#pragma once
#include <SFML/Graphics.hpp>

class Player : public sf::Drawable, public sf::Transformable
{
public:
	Player();

	const sf::Vector2f& getPosition() const;
	void move(const sf::Vector2f& offset);
	sf::FloatRect getGlobalBounds() const;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

