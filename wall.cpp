#include "wall.h"

Wall::Wall(Line line, float thickness, sf::Color color1, sf::Color color2)
	: line(line)
	, thickness(thickness)
{
	resetVerticePositions();

	verticies[0].color = color1;
	verticies[1].color = color1;
	verticies[2].color = color2;
	verticies[3].color = color2;
}

void Wall::render(sf::RenderWindow& window)
{
	window.draw(verticies, 4, sf::TrianglesStrip);
}

void Wall::setX1(TextFieldResult result)
{ 
	line.x1 = result.floatResult;

	resetVerticePositions();
}

void Wall::setY1(TextFieldResult result) 
{ 
	line.y1 = result.floatResult;

	resetVerticePositions();
}

void Wall::setX2(TextFieldResult result) 
{ 
	line.x2 = result.floatResult;

	resetVerticePositions();
}

void Wall::setY2(TextFieldResult result) 
{ 
	line.y2 = result.floatResult;

	resetVerticePositions();
}

void Wall::setThickness(float newThickness)
{
	thickness = newThickness;

	resetVerticePositions();
}

void Wall::setColor1(sf::Color color) 
{ 
	verticies[0].color = color;  
	verticies[1].color = color; 
}

void Wall::setColor2(sf::Color color) 
{ 
	verticies[2].color = color;  
	verticies[3].color = color; 
}

void Wall::resetVerticePositions()
{
	float xOffset = -(line.y2 - line.y1);
	float yOffset = (line.x2 - line.x1);
	const float offsetMagnitude = sqrtf(xOffset * xOffset + yOffset * yOffset);
	xOffset = (xOffset / offsetMagnitude) * thickness;
	yOffset = (yOffset / offsetMagnitude) * thickness;

	verticies[0].position = sf::Vector2f(line.x1 - xOffset, line.y1 - yOffset);
	verticies[1].position = sf::Vector2f(line.x1 + xOffset, line.y1 + yOffset);
	verticies[2].position = sf::Vector2f(line.x2 - xOffset, line.y2 - yOffset);
	verticies[3].position = sf::Vector2f(line.x2 + xOffset, line.y2 + yOffset);
}
