#include "GameWindow.h"

GameWindow::GameWindow(Game& game) :
	game(game), piecesData(),
	deskUpperLeftCorner(sf::Vector2f(80.f, 130.f)),
	cellSize(80.f, 80.f), pieceRadius(35.f) {
	openFlag = false;

	view.reset(sf::FloatRect(0.f, 0.f, 800.f, 800.f));
	view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	sf::RectangleShape cellBuffer;
	cellBuffer.setSize(cellSize);
	cellBuffer.setOutlineThickness(0.f);

	sf::CircleShape pieceBuffer;
	pieceBuffer.setRadius(pieceRadius);
	pieceBuffer.setOutlineThickness(0.f);


	sf::RenderTexture backgroundBuffer;
	backgroundBuffer.create(800, 800);
	backgroundBuffer.clear(sf::Color::Color(70, 70, 70));

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if ((i + j) % 2 == 0) {
				cellBuffer.setFillColor(sf::Color::Color(0, 200, 0));
			} else {
				cellBuffer.setFillColor(sf::Color::Color(0, 150, 0));
			}
			cellBuffer.setPosition(deskUpperLeftCorner + sf::Vector2f(cellSize.x * j, cellSize.y * i));
			backgroundBuffer.draw(cellBuffer);
		}
	}

	pieceBuffer.setPosition(200.f, 30.f);
	pieceBuffer.setFillColor(sf::Color::Black);
	backgroundBuffer.draw(pieceBuffer);

	pieceBuffer.setPosition(520.f, 30.f);
	pieceBuffer.setFillColor(sf::Color::Black);
	backgroundBuffer.draw(pieceBuffer);

	backgroundBuffer.display();

	background = backgroundBuffer.getTexture();

	pieces.create(800, 800);
	pieces.clear(sf::Color::Transparent);
	pieces.display();
	pieces.setActive(false);

	font.loadFromFile("font.ttf");

	blackScore.setFont(font);
	blackScore.setCharacterSize(60);
	blackScore.setPosition(300.f, 30.f);

	whiteScore.setFont(font);
	whiteScore.setCharacterSize(60);
	whiteScore.setPosition(430.f, 30.f);
}

GameWindow::~GameWindow() {
	if (eventHandleThread.joinable()) {
		eventHandleThread.join();
	}
	if (renderThread.joinable()) {
		renderThread.join();
	}
}

void GameWindow::open() {
	openFlag = true;

	std::thread eventHandleThreadBuf(&GameWindow::handleEvents, this);
	std::swap(eventHandleThreadBuf, eventHandleThread);

	std::thread renderThreadBuf(&GameWindow::render, this);
	std::swap(renderThreadBuf, renderThread);
}

bool GameWindow::isOpen() const {
	return openFlag;
}

void GameWindow::handleEvents() {
	window.create(sf::VideoMode(800, 800), "Reversi AI");
	window.setView(view);
	window.setActive(false);

	const sf::FloatRect deskBorder(deskUpperLeftCorner,
								   deskUpperLeftCorner + 8.f * cellSize);

	sf::Event evnt;
	while (window.isOpen()) {
		while (window.pollEvent(evnt)) {
			if (evnt.type == sf::Event::Closed) {
				openFlag = false;
				window.close();
				break;
			}

			if (evnt.type == sf::Event::MouseButtonPressed) {
				if (evnt.mouseButton.button != sf::Mouse::Left) {
					continue;
				}

				sf::Vector2i windowPixel(evnt.mouseButton.x, evnt.mouseButton.y);
				sf::Vector2f clickCoords = window.mapPixelToCoords(windowPixel);

				if (!deskBorder.contains(clickCoords)) {
					continue;
				}

				clickCoords -= deskUpperLeftCorner;
				sf::Vector2f cellCoords(clickCoords.x / cellSize.x,
										clickCoords.y / cellSize.y);

				game.passClick(int(std::floorf(cellCoords.y)),
							   int(std::floorf(cellCoords.x)));
			}
		}

		sf::sleep(sf::milliseconds(delay));
	}

	game.passClick(-1, -1);
}

void GameWindow::render() {
	window.setActive(true);
	pieces.setActive(true);

	sf::Clock timer;
	sf::Sprite backgroundSprite;
	backgroundSprite.setTexture(background, true);
	sf::Sprite piecesSprite;
	piecesSprite.setTextureRect(sf::IntRect(0, 0, 800, 800));

	while (openFlag || window.isOpen()) {
		timer.restart();

		if (!window.isOpen()) {
			sf::sleep(sf::milliseconds(delay));
			continue;
		}

		updateDesk();
		piecesSprite.setTexture(pieces.getTexture());

		window.clear();
		window.draw(backgroundSprite);
		window.draw(piecesSprite);
		window.draw(blackScore);
		window.draw(whiteScore);
		window.display();

		sf::Int32 elapsedTime = timer.getElapsedTime().asMilliseconds();
		if (elapsedTime < delay) {
			sf::sleep(sf::milliseconds(delay - elapsedTime));
		}
	}
}

void GameWindow::updateDesk() {
	auto piecesDataBuffer = game.getCurrentDeskState();
	
	if (piecesDataBuffer != piecesData) {
		sf::CircleShape pieceBuffer;
		pieceBuffer.setRadius(pieceRadius);
		pieceBuffer.setOutlineThickness(0.f);

		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (piecesData[i][j] == piecesDataBuffer[i][j]) {
					continue;
				}

				pieceBuffer.setPosition(deskUpperLeftCorner + sf::Vector2f(cellSize.x * j, cellSize.y * i));
				if (piecesDataBuffer[i][j] == 1) {
					pieceBuffer.setFillColor(sf::Color::Black);
				} else {
					pieceBuffer.setFillColor(sf::Color::White);
				}

				pieces.draw(pieceBuffer);
			}
		}

		pieces.display();
		piecesData = piecesDataBuffer;
	}

	auto [blackPieces, whitePieces] = game.getScore();
	blackScore.setString(std::to_string(blackPieces));
	whiteScore.setString(std::to_string(whitePieces));
}