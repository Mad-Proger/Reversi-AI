#include "GameWindow.h"

GameWindow::GameWindow(Game& game) :
	game(game), piecesData(),
	deskUpperLeftCorner(sf::Vector2f(80.f, 130.f)),
	cellSize(80.f, 80.f), pieceRadius(35.f) {
	openFlag = false;

	view.reset(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(800.f, 800.f)));
	view.setViewport(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(1.f, 1.f)));

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

	pieceBuffer.setPosition(sf::Vector2f(200.f, 30.f));
	pieceBuffer.setFillColor(sf::Color::Black);
	backgroundBuffer.draw(pieceBuffer);

	pieceBuffer.setPosition(sf::Vector2f(520.f, 30.f));
	pieceBuffer.setFillColor(sf::Color::White);
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
	blackScore.setPosition(sf::Vector2f(300.f, 30.f));

	whiteScore.setFont(font);
	whiteScore.setCharacterSize(60);
	whiteScore.setPosition(sf::Vector2f(430.f, 30.f));
}

GameWindow::~GameWindow() {
	if (eventHandleThread.joinable()) {
		eventHandleThread.join();
	}
}

void GameWindow::open() {
	openFlag = true;
	std::thread eventHandleThreadBuffer(&GameWindow::handleEvents, this);
	std::swap(eventHandleThreadBuffer, eventHandleThread);
}

bool GameWindow::isOpen() const {
	return openFlag;
}

void GameWindow::handleEvents() {
	window.create(sf::VideoMode(800, 800), "Reversi AI");
	window.setView(view);
	window.setActive(false);

	std::thread renderThread(&GameWindow::render, this);

	const sf::FloatRect deskBorder(deskUpperLeftCorner,
								   deskUpperLeftCorner + 8.f * cellSize);

	sf::Event evnt;
	while (window.isOpen()) {
		while (window.pollEvent(evnt)) {
			if (evnt.type == sf::Event::Closed) {
				openFlag = false;
				renderThread.join();
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
	piecesSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(800, 800)));

	while (openFlag) {
		timer.restart();

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

		const sf::Vector2f pieceShift = (cellSize - 2.f * sf::Vector2f(pieceRadius, pieceRadius)) / 2.f;

		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (piecesData[i][j] == piecesDataBuffer[i][j]) {
					continue;
				}

				pieceBuffer.setPosition(deskUpperLeftCorner + pieceShift + sf::Vector2f(cellSize.x * j, cellSize.y * i));
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