#include "wall.h"

Wall::Wall(float x1, float y1, float x2, float y2, float thickness, sf::Color color1, sf::Color color2)
	: x1(x1)
	, y1(y1)
	, x2(x2)
	, y2(y2)
{
	float xOffset = -(y2 - y1);
	float yOffset = (x2 - x1);
	const float offsetMagnitude = sqrtf(xOffset * xOffset + yOffset * yOffset);
	xOffset = (xOffset / offsetMagnitude) * thickness;
	yOffset = (yOffset / offsetMagnitude) * thickness;

	verticies[0].position = sf::Vector2f(x1 - xOffset, y1 - yOffset);
	verticies[1].position = sf::Vector2f(x1 + xOffset, y1 + yOffset);
	verticies[2].position = sf::Vector2f(x2 - xOffset, y2 - yOffset);
	verticies[3].position = sf::Vector2f(x2 + xOffset, y2 + yOffset);

	verticies[0].color = color1;
	verticies[1].color = color1;
	verticies[2].color = color2;
	verticies[3].color = color2;
}

void Wall::render(sf::RenderWindow& window)
{
	window.draw(verticies, 4, sf::TrianglesStrip);
}
