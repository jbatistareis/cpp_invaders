#pragma once
#include <iostream>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include "gameInfo.h"
#include "gameScreen.h"

sf::Clock deltaClock;
sf::Time deltaTime;

GameInfo gameInfo;

int main() {
	sf::RenderWindow window(sf::VideoMode(640, 480), "CPP Invaders");
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);

	GameScreen gameScreen = GameScreen(gameInfo);
	gameScreen.start();

	while (window.isOpen()) {
		deltaTime = deltaClock.restart();

		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		window.clear();

		// controls
		if (gameInfo.currentState == gameInfo.PLAYING) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				gameScreen.movePlayerRight(deltaTime.asSeconds());
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				gameScreen.movePlayerLeft(deltaTime.asSeconds());

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				gameScreen.playerShoot(deltaTime.asSeconds());

			gameScreen.draw(window, deltaTime.asSeconds());
		}

		window.display();
	}

	return 0;
}