#pragma once

#include <SFML/Graphics.hpp>

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
	Wall(float x1, float y1, float x2, float y2, float thickness, sf::Color color1, sf::Color color2);

	/* Renders the wall onto the window

	window: A reference to a sf::RenderWindow of which to render the wall onto
	*/
	void render(sf::RenderWindow& window);

	float getX1() { return x1; }
	float getY1() { return y1; }
	float getX2() { return x2; }
	float getY2() { return y2; }
private:
	sf::Vertex verticies[4];

	float x1;
	float y1;
	float x2;
	float y2;
};

/*
A WallGroup is used to group walls based on areas, so only the relevant walls need to be check for collision
*/
struct WallGroup
{
	int count;
	Wall** walls;
};