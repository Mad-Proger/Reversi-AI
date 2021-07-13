#pragma once

#include <array>
#include <string>
#include <thread>

#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include "Game.h"

class GameWindow {
public:
	GameWindow(Game& game);
	~GameWindow();

	void open();

	bool isOpen() const;

private:
	static const sf::Int32 delay = 30;
	const sf::Vector2f deskUpperLeftCorner;
	const sf::Vector2f cellSize;
	const float pieceRadius;

	sf::RenderWindow window;
	bool openFlag;
	sf::View view;

	sf::Texture background;
	sf::RenderTexture pieces;
	std::array<std::array<int, 8>, 8> piecesData;

	sf::Font font;
	sf::Text blackScore;
	sf::Text whiteScore;

	std::thread eventHandleThread;
	
	Game& game;

	void handleEvents();
	void render();
	void updateDesk();
};
