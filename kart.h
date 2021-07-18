#pragma once

#define KART_WIDTH 20
#define KART_LENGTH 30

#define KART_WHEEL_LENGTH 10
#define KART_WHEEL_WIDTH 8

#define KART_WHEEL_ANGLE_LIMIT 40

#include <SFML/Graphics.hpp>

#include <iostream>
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
	const sf::Color WHEEL_COLOR = sf::Color(100, 100, 100, 255);
	const sf::Color DRIFTING_WHEEL_COLOR = sf::Color(170, 170, 170, 255); // this is kind of a temporary solution to indicate drifting, ideally it would be shown with skidmarks or something

	const float KART_ACCLERATION = 0.075f;
	const float KART_TURN_SPEED = 0.0175f;
	const float KART_DRIFT_TURN_MULTI = 1.5f;

	const float KART_DRAG = 0.995f;
	const float KART_SOFT_SPEED_CAP = 5.0f;
	const float KART_SOFT_SPEED_REDUCTION = 0.3f;

	const float KART_DRIFT_START_DRIFT = 0.4f;
	const float KART_DRIFT_STOP_DRIFT = 0.3f;
	const float KART_BASE_START_DRIFT = 1.8f;
	const float KART_BASE_STOP_DRIFT = 1.5f;

	const float KART_DRIFT_MAX_CHANGE = 0.02f;
	const float KART_BASE_MAX_CHANGE = 0.08f;

	// Returns the current movement angle in radians
	float getMovementAngle(float usedAngle);
	// Returns true if the Kart has a positive forward velocity, false otherwise
	bool isMovingForward();
	// Updates xVelocity and yVelocity, by calculating the friction of the wheels, which encourages the car to move in the direction of the wheels
	void doFriction();

	sf::RectangleShape body;
	sf::RectangleShape wheel;

	Input* input;

	float xPosition;
	float yPosition;
	float angle; // in degrees

	float wheelAngle; // in degrees, relative to angle

	float xVelocity; // units/s
	float yVelocity; // units/s
	float angularVelocity; // deg/s | from collisions, other angle adjustments are going to come from direct calculations with wheel angle and whatnot

	bool isDrifting;
};