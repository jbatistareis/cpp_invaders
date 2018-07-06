#include <SFML/Graphics.hpp>

class Enemy
{
public:
	bool alive = true;
	bool hit = false;
	sf::CircleShape shape = sf::CircleShape(15);

	Enemy();
	Enemy(double x, double y);
};
