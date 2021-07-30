#include "wall.h"

Wall::Wall(float x1, float y1, float x2, float y2, float thickness, sf::Color color1, sf::Color color2)
	: thickness(thickness)
	, x1(x1)
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

float Wall::distanceToLine(float px, float py, float wx1, float wy1, float wx2, float wy2)
{
	float lineWX = wx1 - wx2;
	float lineWY = wy1 - wy2;

	float distance = lineWX * lineWX + lineWY * lineWY;
	float dot = (px - wx2) * lineWX + (py - wy2) * lineWY;
	float ratio = dot / distance;
	if (ratio < 0.0f)
	{
		ratio = 0.0f;
	}
	else if (ratio > 1.0f)
	{
		ratio = 1.0f;
	}

	float closestX = wx2 + lineWX * ratio;
	float closestY = wy2 + lineWY * ratio;

	return sqrtf((px - closestX) * (px - closestX) + (py - closestY) * (py - closestY));
}

void Wall::setX1(TextFieldResult result)
{ 
	x1 = result.floatResult; 

	float xOffset = -(y2 - y1);
	float yOffset = (x2 - x1);
	const float offsetMagnitude = sqrtf(xOffset * xOffset + yOffset * yOffset);
	xOffset = (xOffset / offsetMagnitude) * thickness;
	yOffset = (yOffset / offsetMagnitude) * thickness;

	verticies[0].position = sf::Vector2f(x1 - xOffset, y1 - yOffset);
	verticies[1].position = sf::Vector2f(x1 + xOffset, y1 + yOffset);
}

void Wall::setY1(TextFieldResult result) 
{ 
	y1 = result.floatResult; 

	float xOffset = -(y2 - y1);
	float yOffset = (x2 - x1);
	const float offsetMagnitude = sqrtf(xOffset * xOffset + yOffset * yOffset);
	xOffset = (xOffset / offsetMagnitude) * thickness;
	yOffset = (yOffset / offsetMagnitude) * thickness;

	verticies[0].position = sf::Vector2f(x1 - xOffset, y1 - yOffset);
	verticies[1].position = sf::Vector2f(x1 + xOffset, y1 + yOffset);
}

void Wall::setX2(TextFieldResult result) 
{ 
	x2 = result.floatResult; 

	float xOffset = -(y2 - y1);
	float yOffset = (x2 - x1);
	const float offsetMagnitude = sqrtf(xOffset * xOffset + yOffset * yOffset);
	xOffset = (xOffset / offsetMagnitude) * thickness;
	yOffset = (yOffset / offsetMagnitude) * thickness;

	verticies[2].position = sf::Vector2f(x2 - xOffset, y2 - yOffset);
	verticies[3].position = sf::Vector2f(x2 + xOffset, y2 + yOffset);
}

void Wall::setY2(TextFieldResult result) 
{ 
	y2 = result.floatResult; 

	float xOffset = -(y2 - y1);
	float yOffset = (x2 - x1);
	const float offsetMagnitude = sqrtf(xOffset * xOffset + yOffset * yOffset);
	xOffset = (xOffset / offsetMagnitude) * thickness;
	yOffset = (yOffset / offsetMagnitude) * thickness;

	verticies[2].position = sf::Vector2f(x2 - xOffset, y2 - yOffset);
	verticies[3].position = sf::Vector2f(x2 + xOffset, y2 + yOffset);
}

void Wall::setThickness(float newThickness)
{
	thickness = newThickness;

	float xOffset = -(y2 - y1);
	float yOffset = (x2 - x1);
	const float offsetMagnitude = sqrtf(xOffset * xOffset + yOffset * yOffset);
	xOffset = (xOffset / offsetMagnitude) * thickness;
	yOffset = (yOffset / offsetMagnitude) * thickness;

	verticies[0].position = sf::Vector2f(x1 - xOffset, y1 - yOffset);
	verticies[1].position = sf::Vector2f(x1 + xOffset, y1 + yOffset);
	verticies[2].position = sf::Vector2f(x2 - xOffset, y2 - yOffset);
	verticies[3].position = sf::Vector2f(x2 + xOffset, y2 + yOffset);
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
