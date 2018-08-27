#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:
	Player();
	void moveLeft(float& deltaTime);
	void moveRight(float& deltaTime);
	void shoot(float& deltaTime);
	void resetPositon();
	int getLives();
	void reduceLive();
	void oneUp();
	std::vector<sf::RectangleShape>& getProjectiles();
	void outsideProjCountAdd();
	void outsideProjCountReset();
	int getOutsideProjCount();
	double getProjSpeed();
	sf::FloatRect getBounds();
	void draw(sf::RenderWindow& window, float& deltaTime);

private:
	// replace for sprite
	sf::RectangleShape player;
	std::vector<sf::RectangleShape> projectiles;
	int outsideProjCount = 0;
	int lives = 3;
	bool hit = false;
	double timeSinceLastShot = 0;
	double speed = 250;
	double projSpeed = 4;
	double shotInvterval = 0.5;
	
};

