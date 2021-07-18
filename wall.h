#pragma once

#include <SFML/Graphics.hpp>

/*
A Wall is a line segment that the kart is able to collide with. They are used as obsticals and barriers that make up the track.
*/
class Wall
{
public:
	Wall(float x1, float y1, float x2, float y2, float thickness, sf::Color color1, sf::Color color2);

	void render(sf::RenderWindow& window);

private:
	sf::Vertex verticies[4];
};