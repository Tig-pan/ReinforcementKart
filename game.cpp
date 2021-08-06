#include "game.h"

Game::Game(std::string mapFileName, Input* input, sf::RenderWindow& window, sf::Font& font)
	: window(window)
	, font(font)

	, lapLabel(LabelAlign::RightTop, "", 32, font, Transform(-10, -10, 10, 10, 1.0f, 1.0f, 0.0f, 0.0f), window)
	, lapProgressLabel(LabelAlign::RightTop, "", 24, font, Transform(-10, -10, 50, 50, 1.0f, 1.0f, 0.0f, 0.0f), window)
{
	// Load map
	std::ifstream mapFile;
	mapFile.open(mapFileName, std::ios::in | std::ios::binary);

	mapFile.read((char*)&startAngle, sizeof(startAngle));

	mapFile.read((char*)&wallCount, sizeof(wallCount));

	walls = new Wall*[wallCount];

	float x1;
	float y1;
	float x2;
	float y2;
	uint8_t r1;
	uint8_t g1;
	uint8_t b1;
	uint8_t r2;
	uint8_t g2;
	uint8_t b2;

	for (int i = 0; i < wallCount; i++)
	{
		mapFile.read((char*)&x1, sizeof(x1));
		mapFile.read((char*)&y1, sizeof(y1));
		mapFile.read((char*)&x2, sizeof(x2));
		mapFile.read((char*)&y2, sizeof(y2));

		mapFile.read((char*)&r1, sizeof(r1));
		mapFile.read((char*)&g1, sizeof(g1));
		mapFile.read((char*)&b1, sizeof(b1));

		mapFile.read((char*)&r2, sizeof(r2));
		mapFile.read((char*)&g2, sizeof(g2));
		mapFile.read((char*)&b2, sizeof(b2));

		walls[i] = new Wall(Line(x1, y1, x2, y2), DEFAULT_WALL_THICKNESS, sf::Color(r1, g1, b1), sf::Color(r2, g2, b2));
	}

	mapFile.read((char*)&checkpointCount, sizeof(checkpointCount));

	checkpoints = new Checkpoint*[checkpointCount];

	int wallsInGroup;
	int index;

	for (int i = 0; i < checkpointCount; i++)
	{
		mapFile.read((char*)&x1, sizeof(x1));
		mapFile.read((char*)&y1, sizeof(y1));
		mapFile.read((char*)&x2, sizeof(x2));
		mapFile.read((char*)&y2, sizeof(y2));

		mapFile.read((char*)&wallsInGroup, sizeof(wallsInGroup));

		Wall** wallGrouping = new Wall*[wallsInGroup];

		for (int j = 0; j < wallsInGroup; j++)
		{
			mapFile.read((char*)&index, sizeof(index));
			wallGrouping[j] = walls[index];
		}

		checkpoints[i] = new Checkpoint(Line(x1, y1, x2, y2), wallGrouping, wallsInGroup);
	}
	// Done loading map

	kart = new Kart("Game", input, sf::Color::Yellow, 0, 0, startAngle, checkpoints, checkpointCount);
	guiView = sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
}

void Game::run()
{
	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (e.type == sf::Event::Resized)
			{
				guiView = sf::View(sf::FloatRect(0, 0, e.size.width, e.size.height));
				window.setView(sf::View(sf::FloatRect(0, 0, e.size.width, e.size.height)));
			}

			//button.handleEvent(window, e);
			//tf.handleEvent(window, e);
		}

		kart->tick();
		kart->update();
		gameView = sf::View(sf::FloatRect(
			kart->getX() - window.getSize().x * 0.5f,
			kart->getY() - window.getSize().y * 0.5f,
			window.getSize().x,
			window.getSize().y));

		updateGUI();

		window.clear();
		window.setView(gameView);
		for (int i = 0; i < wallCount; i++)
		{
			walls[i]->render(window);
		}
		kart->render(window);

		window.setView(guiView);
		lapLabel.render(window);
		lapProgressLabel.render(window);

		window.display();
	}
}

void Game::updateGUI()
{
	lapLabel.setText("Lap: " + std::to_string(kart->getLapNumber() + 1), window);
	std::ostringstream ss; // have to use string stream because its the only way that formats properly
	ss << std::floor(kart->getLapProgress() * 100.0f) << "%";
	lapProgressLabel.setText(ss.str(), window);
}
