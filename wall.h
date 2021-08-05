#pragma once

#include <SFML/Graphics.hpp>

#include "line.h"
#include "gui.h"

/*
A Wall is a line segment that the kart is able to collide with. They are used as obsticals and barriers that make up the track.
*/
class Wall
{
public:
	/* A constructor for Wall, contains parameters for positions, and for visual aspects such as thickness and colors

	x1, y1, x2, y2: floats representing the points of the wall
	thickness: the visual thickness of the wall (note: the collision detection will treat the wall as being infinitly thin)
	color1, color2: the color of the wall at each point
	*/
	Wall(Line line, float thickness, sf::Color color1, sf::Color color2);

	/* Renders the wall onto the window

	window: A reference to a sf::RenderWindow of which to render the wall onto
	*/
	void render(sf::RenderWindow& window);

	void setX1(TextFieldResult result);
	void setY1(TextFieldResult result);
	void setX2(TextFieldResult result);
	void setY2(TextFieldResult result);

	void setThickness(float newThickness);

	void setColor1(sf::Color color);
	void setColor2(sf::Color color);

	Line getLine() { return line; }

	sf::Color getColor1() { return verticies[0].color; }
	sf::Color getColor2() { return verticies[2].color; }
private:
	void resetVerticePositions();

	Line line;

	sf::Vertex verticies[4];

	float thickness;
};