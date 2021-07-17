#pragma once

#define KART_WIDTH 20
#define KART_LENGTH 30

#include <SFML/Graphics.hpp>

#include <string>

#include "input.h"
#include "constants.h"

/*
A Kart controlled by an Input. The Karts are not made to act super realistically, just to be controlled in a
fun way that also allows for interesting tech and difficult to perform manuvers. To make sure the movement is fun
yet challenging. Especially the physics with colliding isn't going to be super realistic.
*/
class Kart
{
public:
	/* A constructor for Kart, contains parameters for a racerName, a pointer to an Input object, a color for the kart, and a starting position

	racerName: a String for the name of the racer in the kart
	input: a pointer to an Input object to be controlling this kart
	kartColor: the sf::Color of the kart
	startX: a float representing the initial x position of the kart
	startY: a float representing the initial y position of the kart
	startAngle: a float representing the initial angle of the kart (in degrees)
	*/
	Kart(std::string racerName, Input* input, sf::Color kartColor, float startX, float startY, float startAngle);

	// Called every frame, updates the Karts position based on its physics
	void update();

	/* Renders the kart onto the window

	window: A reference to a sf::RenderWindow of which to render the kart onto
	*/
	void render(sf::RenderWindow& window);

private:
	sf::RectangleShape rect;

	Input* input;

	float xPosition;
	float yPosition;
	float angle; // in degrees

	float wheelAngle; // in degrees, relative to angle

	float xVelocity; // units/s
	float yVelocity; // units/s
	float angularVelocity; // deg/s | from collisions, other angle adjustments are going to come from direct calculations with wheel angle and whatnot
};