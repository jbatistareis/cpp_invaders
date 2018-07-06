#include <SFML/Graphics.hpp>

struct Enemy {
	bool alive;
	bool hit;
	sf::CircleShape shape;
};