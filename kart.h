#pragma once

#define KART_WHEEL_ANGLE_LIMIT 40

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

#include "input.h"
#include "constants.h"
#include "wall.h"
#include "checkpoint.h"

/*
* An enum to help with collision and other tasks related to the corners of the kart
*/
enum class Corner
{
	FrontLeft,
	FrontRight,
	BackLeft,
	BackRight
};

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
	Kart(std::string racerName, Input* input, KartSensors* sensors, sf::Color kartColor, float startX, float startY, float startAngle, Checkpoint** race, int checkpointCount);

	// Called every frame, updates the Karts velocity based on inputs
	void tick();

	// Called every frame, updates the Karts position based on its physics, called after all karts have done tick
	void update();

	/* Renders the kart onto the window

	window: A reference to a sf::RenderWindow of which to render the kart onto
	*/
	void render(sf::RenderWindow& window);

	// Returns the x position of the kart
	float getX() { return xPosition; }

	// Returns the y position of the kart
	float getY() { return yPosition; }

	// Resets the position and angle of the kart to the given values, also sets velocities to 0
	void resetPosition(float newXPosition, float newYPosition, float newAngle);

	// Returns the current lap number
	int getLapNumber() { return lapNumber; }

	// Returns a number from 0-1 representing the percentage of the current lap completed
	float getLapProgress();

	// Returns the vision group of walls for the current checkpoint
	Wall** getVisionCheckpointGroup() { return race[currentCheckpoint]->getVisionWallGroup(); }

	// Returns the amount of walls in the vision group for the current checkpoint
	int getVisionCheckpointGroupSize() { return race[currentCheckpoint]->getWallsInVisionGroup(); }

private:
	const sf::Color WHEEL_COLOR = sf::Color(100, 100, 100, 255);
	const sf::Color DRIFTING_WHEEL_COLOR = sf::Color(170, 170, 170, 255); // this is kind of a temporary solution to indicate drifting, ideally it would be shown with skidmarks or something

	const float KART_ACCLERATION = 0.075f;
	const float KART_HANDLING = 2.5f;
	const float KART_TURN_SPEED = 0.0175f;
	const float KART_DRIFT_TURN_MULTI = 1.5f;

	const float KART_DRAG = 0.995f;
	const float KART_SOFT_SPEED_CAP = 4.0f;
	const float KART_SOFT_SPEED_REDUCTION = 0.3f;

	const float KART_DRIFT_START_DRIFT = 0.2f;
	const float KART_DRIFT_STOP_DRIFT = 0.1f;
	const float KART_BASE_START_DRIFT = 2.0f;
	const float KART_BASE_STOP_DRIFT = 1.7f;

	const float KART_DRIFT_MAX_CHANGE = 0.03f;
	const float KART_BASE_MAX_CHANGE = 0.105f;

	const float KART_BASE_BOUNCE_REDUCE = 0.15f;
	const float KART_ELASTICITY = 0.3f;
	const float KART_SPIN_MULTIPLIER = 0.05f;
	const float KART_SPIN_FALLOFF = 0.94f;

	// Returns the angle between the vector and the given angle in radians, uses the usedAngle to compare against the vector
	float getAngleBetween(float vectorX, float vectorY, float usedAngle);
	// Returns true if the Kart has a positive forward velocity, false otherwise
	bool isMovingForward();
	// Updates xVelocity and yVelocity, by calculating the friction of the wheels, which encourages the car to move in the direction of the wheels
	void doFriction();

	// Checks for collisions with walls and other karts before executing the movement
	void doCollisions();
	// Checks for passing checkpoints in order to progress in the race and 
	void doCheckpoints(float xPositionBefore, float yPositionBefore);

	sf::RectangleShape body;
	sf::RectangleShape wheel;

	Input* input;
	KartSensors* sensors;
	Checkpoint** race;
	int checkpointCount;

	int currentCheckpoint;
	int furthestCheckpoint;
	int lapNumber;

	float checkpointProgress;

	float xPosition;
	float yPosition;
	float angle; // in degrees

	float wheelAngle; // in degrees, relative to angle

	float xVelocity; // units/s
	float yVelocity; // units/s
	float angularVelocity; // deg/s
	float kartSpin; // deg/s

	bool isDrifting;
};