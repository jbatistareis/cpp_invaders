#pragma once
#include <SFML/Graphics.hpp>

class GameScreen : public sf::Drawable
{
public:
	GameScreen();

private:
	void start();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

